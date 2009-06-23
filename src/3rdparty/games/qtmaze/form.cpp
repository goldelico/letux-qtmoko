/*  form.cpp
 *
 *  Main game routines
 *
 *  (c) 2009 Anton Olkhovik <ant007h@gmail.com>
 *
 *  This file is part of QtMaze (port of Mokomaze) - labyrinth game.
 *
 *  QtMaze is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QtMaze is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QtMaze.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <qtimer.h>
#include <QMouseEvent>
#include <qsoftmenubar.h>
#include <QtopiaApplication>
#include <stdio.h>
#include <math.h>
#include "accelerometers.h"
#include "paramsloader.h"
#include "vibro.h"
#include "types.h"
#include "form.h"

//============================================================================

#define GRAV_CONST 9.8*1.0
#define FRICT_COEF 0.12 //0.20
#define TIME_QUANT 1.8
#define SPEED_TO_PIXELS 0.05
#define FORCE_TREASURE 0.040 //0.055
#define BUMP_COEF 0.3

#define PROC_ACC_DATA_INTERVAL 20

double px,py;
double vx,vy;
double pr_px, pr_py;
double pr_vx, pr_vy;

double prev_px, prev_py;

bool fullscreen = true;

#define FULLSCREEN_NONE   0
#define FULLSCREEN_TOGGLE 1
#define FULLSCREEN_ALWAYS 2

Config qt_game_config;
Level* qt_game_levels;
int qt_game_levels_count;
int cur_level;

#define GAME_STATE_NORMAL   1
#define GAME_STATE_FAILED   2
#define GAME_STATE_WIN      3
int game_state;
int new_game_state;

#define FASTCHANGE_INTERVAL 1000
int fastchange_step = 0;


void Form::DisableScreenSaver()
{
    QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
}

void Form::EnableScreenSaver()
{
    QtopiaApplication::setPowerConstraint(QtopiaApplication::Enable);
}

void Form::SetLevelNo()
{
    char txt[256];
    sprintf(txt, "<font color=\"#e0bc70\">Level %d/%d</font>", cur_level+1, qt_game_levels_count);
    levelno_lbl->setText(txt);
    //levelno_lbl->setText(QApplication::translate("FormBase", txt, 0, QApplication::UnicodeUTF8));
}

void Form::SetMenuVis(bool x)
{
    SetLevelNo();

    levelno_lbl->setVisible(x);
    info1_lbl->setVisible(x);
    prev_lbl->setVisible(x);
    next_lbl->setVisible(x);
    reset_lbl->setVisible(x);
    exit_lbl->setVisible(x);
}

bool InRect(int px,int py,  int x,int y,int w,int h)
{
    return (
                (px>=x)&&(px<=x+w) &&
                (py>=y)&&(py<=y+h)
           );
}

//============================================================================

void Form::BumpVibrate(double speed)
{
    //printf("%.4f\n", speed);
    #define MAX_BUMP_SPEED 160.0
    #define MIN_BUMP_SPEED 45
    //#define MAX_BUMP_SPEED 160.0
    //#define VIB_TRESHHOLD 0.30
    double v = fabs(speed);
    double k = v/MAX_BUMP_SPEED;
    if (k>1) k=1;
    //if (k>VIB_TRESHHOLD)
    if (v>MIN_BUMP_SPEED)
    {
        BYTE vlevel = (BYTE)(k*255);
        set_vibro(vlevel);
    }
}

void Form::post_phys_res(double x, double y, double mm_vx, double mm_vy)
{
    px = x; py = y;
    MoveBall(px, py); //

    vx = mm_vx; vy = mm_vy;
    pr_px = px; pr_py = py;
    pr_vx = vx; pr_vy = vy;
}

double tmp_px, tmp_py;
double tmp_vx, tmp_vy;
void Form::post_temp_phys_res(double x, double y, double mm_vx, double mm_vy)
{
    if (x<qt_game_config.ball_r)
    {
        BumpVibrate(mm_vx); //VIB_HOR
        x = qt_game_config.ball_r;
        mm_vx = -mm_vx * BUMP_COEF;
    }
    if (x>qt_game_config.wnd_w - qt_game_config.ball_r)
    {
        BumpVibrate(mm_vx);
        x = qt_game_config.wnd_w - qt_game_config.ball_r;
        mm_vx = -mm_vx * BUMP_COEF;
    }
    if (y<qt_game_config.ball_r)
    {
        BumpVibrate(mm_vy);
        y = qt_game_config.ball_r;
        mm_vy = -mm_vy * BUMP_COEF;
    }
    if (y>qt_game_config.wnd_h - qt_game_config.ball_r)
    {
        BumpVibrate(mm_vy);
        y = qt_game_config.wnd_h - qt_game_config.ball_r;
        mm_vy = -mm_vy * BUMP_COEF;
    }

    tmp_px = x; tmp_py = y;
    tmp_vx = mm_vx; tmp_vy = mm_vy;
}

void Form::apply_temp_phys_res()
{
    post_phys_res(tmp_px,tmp_py, tmp_vx,tmp_vy);
}

void Form::tout()
{
    //printf("[acc] %.4f %.4f %.4f\n", getacx(), getacy(), getacz());

    if ( (game_state != GAME_STATE_NORMAL) || (!fullscreen) ) return;
    new_game_state = GAME_STATE_NORMAL;

    double ax = getacx();
    double ay = getacy();

    double mid_px=px, mid_py=py;
    double mid_vx=vx, mid_vy=vy;

    double v = sqrt(mid_vx*mid_vx+mid_vy*mid_vy);
    double a = sqrt(ax*ax+ay*ay);
    if ((v > 0) || (a > FORCE_TREASURE))
    {
            mid_vx += ax*GRAV_CONST * TIME_QUANT;
            mid_vy += ay*GRAV_CONST * TIME_QUANT;
    }
    if (fabs(mid_vx) > 0)
    {
            double dvx = fabs( FRICT_COEF * GRAV_CONST*cos(asin(ax)) );
            if (mid_vx>0)
            {
                    mid_vx-=dvx;
                    if (mid_vx<0) mid_vx=0;
            }
            else
            {
                    mid_vx+=dvx;
                    if (mid_vx>0) mid_vx=0;
            }
    }
    if (fabs(mid_vy) > 0)
    {
            double dvy = fabs( FRICT_COEF * GRAV_CONST*cos(asin(ay)) );
            if (mid_vy>0)
            {
                    mid_vy-=dvy;
                    if (mid_vy<0) mid_vy=0;
            }
            else
            {
                    mid_vy+=dvy;
                    if (mid_vy>0) mid_vy=0;
            }
    }

    mid_px +=  (mid_vx * SPEED_TO_PIXELS);
    mid_py += -(mid_vy * SPEED_TO_PIXELS);

    if (!line(pr_px,pr_py, mid_px,mid_py,    pr_vx,pr_vy, mid_vx,mid_vy))
    {
        post_temp_phys_res(mid_px,mid_py, mid_vx,mid_vy);
        apply_temp_phys_res();
    }
}

double calcdist(double x1,double y1, double x2,double y2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

double calclen(double x, double y)
{
    return sqrt(x*x + y*y);
}

DPoint normalize(DPoint p)
{
    DPoint r;
    double len = calclen(p.x, p.y);
    r.x = p.x/len;
    r.y = p.y/len;
    return r;
}

int inbox(double x, double y, Box box)
{
    return ( (x>=box.x1) && (x<=box.x2) &&
             (y>=box.y1) && (y<=box.y2) );
}

int incircle(double x, double y,  double cx, double cy, double cr)
{
    return (calcdist(x,y, cx,cy) <= cr);
}

#define MAX_TRUEDIR_STEPS 5
int Form::edgebump(int tx,int ty,   double x,double y,   double mm_vx,double mm_vy)
{
    Point touch;
    touch.x = tx; touch.y = ty;
    if (incircle(touch.x,touch.y, x,y,qt_game_config.ball_r))
    {
        DPoint tmp_norm;
        tmp_norm.x = (x - touch.x);
        tmp_norm.y = (y - touch.y);
        DPoint norm = normalize(tmp_norm);
        mm_vy = -mm_vy;
        double dot_norm_vect = mm_vx*norm.x + mm_vy*norm.y;
        mm_vx = mm_vx - 2*norm.x*dot_norm_vect;
        mm_vy = mm_vy - 2*norm.y*dot_norm_vect;

        double vpr_x = -fabs(dot_norm_vect)*norm.x;
        double vpr_y = -fabs(dot_norm_vect)*norm.y;
        BumpVibrate(calclen(vpr_x, vpr_y));

        double dop_x = (1-BUMP_COEF) * vpr_x;
        double dop_y = (1-BUMP_COEF) * vpr_y;
        mm_vx = mm_vx + dop_x;
        mm_vy = mm_vy + dop_y;
        mm_vy = -mm_vy;

        x = touch.x + norm.x*(qt_game_config.ball_r+0.10);
        y = touch.y + norm.y*(qt_game_config.ball_r+0.10);

        post_temp_phys_res(x,y, mm_vx,mm_vy);
        return 1;
    }
    return 0;
}

#define HOLE_SCALE 1.00
int Form::testbump(double x,double y,   double mm_vx,double mm_vy)
{

    Point final_hole = qt_game_levels[cur_level].fins[0];
    double dist = calcdist(x,y, final_hole.x,final_hole.y);
    if (dist <= qt_game_config.hole_r * HOLE_SCALE)
    {
        new_game_state = GAME_STATE_WIN;
        return 1;
    }

    for (int i=0; i<qt_game_levels[cur_level].holes_count; i++)
    {
        Point hole = qt_game_levels[cur_level].holes[i];
        Box boundbox;
        boundbox.x1 = hole.x - qt_game_config.hole_r*HOLE_SCALE -1;
        boundbox.y1 = hole.y - qt_game_config.hole_r*HOLE_SCALE -1;
        boundbox.x2 = hole.x + qt_game_config.hole_r*HOLE_SCALE +1;
        boundbox.y2 = hole.y + qt_game_config.hole_r*HOLE_SCALE +1;
        if (inbox(x,y, boundbox))
        {
            double dist = calcdist(x,y, hole.x,hole.y);
            if (dist <= qt_game_config.hole_r * HOLE_SCALE)
            {
                new_game_state = GAME_STATE_FAILED;
                return 1;
            }
        }
    }

    int retval = 0;

    for (int i=0; i<qt_game_levels[cur_level].boxes_count; i++)
    {
        Box box = qt_game_levels[cur_level].boxes[i];
        Box boundbox;
        boundbox.x1 = box.x1 - qt_game_config.hole_r*HOLE_SCALE -1;
        boundbox.y1 = box.y1 - qt_game_config.hole_r*HOLE_SCALE -1;
        boundbox.x2 = box.x2 + qt_game_config.hole_r*HOLE_SCALE +1;
        boundbox.y2 = box.y2 + qt_game_config.hole_r*HOLE_SCALE +1;
        if (inbox(x,y, boundbox))
        {
            if (inbox(x,y-qt_game_config.ball_r, box))
            {
                BumpVibrate(mm_vy);
                y = box.y2+qt_game_config.ball_r+0.2;
                mm_vy = -mm_vy * BUMP_COEF;
                retval = 1;
            }
            if (inbox(x,y+qt_game_config.ball_r-0, box))
            {
                BumpVibrate(mm_vy);
                y = box.y1-qt_game_config.ball_r -1.00;
                mm_vy = -mm_vy * BUMP_COEF;
                retval = 1;
            }
            if (inbox(x-qt_game_config.ball_r,y, box))
            {
                BumpVibrate(mm_vx);
                x = box.x2+qt_game_config.ball_r+0.2;
                mm_vx = -mm_vx * BUMP_COEF;
                retval = 1;
            }
            if (inbox(x+qt_game_config.ball_r-0,y, box))
            {
                BumpVibrate(mm_vx);
                x = box.x1-qt_game_config.ball_r -1.00;
                mm_vx = -mm_vx * BUMP_COEF;
                retval = 1;
            }

            if (edgebump(box.x1,box.y1, x,y, mm_vx,mm_vy)) return 1;
            if (edgebump(box.x2,box.y1, x,y, mm_vx,mm_vy)) return 1;
            if (edgebump(box.x2,box.y2, x,y, mm_vx,mm_vy)) return 1;
            if (edgebump(box.x1,box.y2, x,y, mm_vx,mm_vy)) return 1;
        }

    }

    if (retval)
    {
        post_temp_phys_res(x,y, mm_vx,mm_vy);
    }

    return retval;
}

void Form::ProcessGameState()
{
    if (game_state == GAME_STATE_FAILED)
    {
        InitState();
        game_state = GAME_STATE_NORMAL;
    }

    if (game_state == GAME_STATE_WIN)
    {
        if (cur_level == qt_game_levels_count-1)
        {
            cur_level=0;
        }
        else
        {
            cur_level++;
        }
        InitState();
        game_state = GAME_STATE_NORMAL;
    }
}

#define MAX_PHYS_ITERATIONS 10
int Form::line(double x0, double y0, double x1, double y1,    double vx0,double vy0, double vx1,double vy1)
{

    double x=x0, y=y0;
    double mm_vx=vx1, mm_vy=vy1; //

    DPoint vec;
    vec.x = x1-x0;
    vec.y = y1-y0;
    DPoint norm_vec;
    double len = calclen(vec.x, vec.y);

    norm_vec.x = vec.x/len;
    norm_vec.y = vec.y/len;
    double k=0;
    int muststop=0;
    while (1)
    {
        k += 1;
        if (k>=len)
        {
            k=len;
            muststop=1;
        }
        x = x0 + norm_vec.x*k;
        y = y0 + norm_vec.y*k;
        if (testbump(x,y,  mm_vx,mm_vy))
        {
            int ko=0;
            while (new_game_state == GAME_STATE_NORMAL)
            {
                ko++;
                //TODO: lite testbump version
                int bump = testbump(tmp_px,tmp_py,  tmp_vx,tmp_vy);
                if ( (!bump) || (ko >= MAX_PHYS_ITERATIONS) ) break;
            }
            apply_temp_phys_res();
            game_state = new_game_state;
            ProcessGameState();
            return 1;
        }
        if (muststop) break;
    }

    return 0;

}

void Form::InitState()
{
    renderArea->setLevel(cur_level);
    renderArea->update();

    px=qt_game_levels[cur_level].init.x; py=qt_game_levels[cur_level].init.y;
    vx=0; vy=0;

    pr_px=px; pr_py=py;
    pr_vx=0; pr_vy=0;

    prev_px=px; prev_py=py;
    MoveBall(px, py); //
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void Form::MoveBall(double x, double y)
{
    ball_lbl->move((int)x-qt_game_config.ball_r, (int)y-qt_game_config.ball_r);
}

void Form::acc_timerAction()
{
    tout();
}

void Form::timerAction()
{
    int new_cur_level = cur_level + fastchange_step;
    if (new_cur_level >= qt_game_levels_count) new_cur_level = qt_game_levels_count-1;
    if (new_cur_level < 0) new_cur_level = 0;
    if (new_cur_level != cur_level)
    {
        cur_level = new_cur_level;
        InitState();
        game_state = GAME_STATE_NORMAL;
        SetLevelNo();
    }
}

bool Form::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (timer->isActive()) timer->stop();
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = (QMouseEvent *) event;
        int mx, my;
        mx = mouseEvent->pos().x();
        my = mouseEvent->pos().y();

        //printf("%d %d\n",mx,my);

        if (fullscreen)
        {
            SetMenuVis(true);
            if (qt_game_config.fullscreen == FULLSCREEN_TOGGLE) setWindowState( windowState() & ~Qt::WindowFullScreen );
            fullscreen = false;
        }
        else
        {
            if (InRect(mx, my,
                       prev_lbl->pos().x(), prev_lbl->pos().y(),
                       prev_lbl->size().width(), prev_lbl->size().height() ))
            {
                if (cur_level > 0)
                {
                    cur_level--;
                    InitState();
                    game_state = GAME_STATE_NORMAL;
                    SetLevelNo();

                    fastchange_step = -10;
                    if (timer->isActive()) timer->stop();
                    timer->start(FASTCHANGE_INTERVAL);
                }
            }
            else if (InRect(mx, my,
                       next_lbl->pos().x(), next_lbl->pos().y(),
                       next_lbl->size().width(), next_lbl->size().height() ))
            {
                if (cur_level < qt_game_levels_count-1)
                {
                    cur_level++;
                    InitState();
                    game_state = GAME_STATE_NORMAL;
                    SetLevelNo();

                    fastchange_step = +10;
                    if (timer->isActive()) timer->stop();
                    timer->start(FASTCHANGE_INTERVAL);
                }
            }
            else if (InRect(mx, my,
                       reset_lbl->pos().x(), reset_lbl->pos().y(),
                       reset_lbl->size().width(), reset_lbl->size().height() ))
            {
                cur_level=0;
                InitState();
                game_state = GAME_STATE_NORMAL;
                SetLevelNo();
            }
            else if (InRect(mx, my,
                       exit_lbl->pos().x(), exit_lbl->pos().y(),
                       exit_lbl->size().width(), exit_lbl->size().height() ))
            {
                close();
            }
            else //no buttons pressed
            {
                SetMenuVis(false);
                if (qt_game_config.fullscreen == FULLSCREEN_TOGGLE) setWindowState( windowState() | Qt::WindowFullScreen );
                fullscreen = true;
            }
        }

        return true;    //event was processed, no futher processing needed
    }
    else if (event->type() == QEvent::Close)
    {
        SaveLevel(cur_level+1);
        accelerometer_stop();
        close_vibro();
        EnableScreenSaver();
    }

    return false;    //event must be processed by parent widget
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

/*
void Form::CheckLoadedPictures()
{
    if (prev_lbl->pixmap()->isNull()) prev_lbl->setPixmap(QPixmap(":image/prev.png"));
    if (next_lbl->pixmap()->isNull()) next_lbl->setPixmap(QPixmap(":image/next.png"));
    if (reset_lbl->pixmap()->isNull()) reset_lbl->setPixmap(QPixmap(":image/reset.png"));
    if (exit_lbl->pixmap()->isNull()) exit_lbl->setPixmap(QPixmap(":image/close.png"));

    if (ball_lbl->pixmap()->isNull()) ball_lbl->setPixmap(QPixmap(":image/ball.png"));

    if (renderArea->hole_pixmap.isNull()) renderArea->hole_pixmap.load(":image/hole.png");
    if (renderArea->fin_pixmap.isNull()) renderArea->fin_pixmap.load(":image/fin.png");
}
*/

/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
Form::Form(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{

    if (load_params() < 0) exit(1);
    qt_game_config = GetGameConfig();
    qt_game_levels = GetGameLevels();
    qt_game_levels_count = GetGameLevelsCount();
    if (qt_game_levels_count<=0) exit(1);
    cur_level = GetUserSettings().level - 1;
    if (cur_level < 0) cur_level = 0;
    if (cur_level >= qt_game_levels_count) cur_level = qt_game_levels_count - 1;
    game_state = GAME_STATE_NORMAL;
    if (GetVibroEnabled()) init_vibro();

    renderArea = new RenderArea(this);
    renderArea->move(0,0);
    renderArea->resize(480,640);  //vboxLayout->addWidget(renderArea);
    renderArea->setLevel(cur_level);

    setupUi(this); // reads the .ui file to determine widgets and layout
    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
    Q_UNUSED(menu); // Normally, we would use "menu" to add more actions.
    // The context menu will not be active unless it has at least one action.
    // Tell Qtopia to provide the "Help" option, which will enable the user to
    // read the Help documentation for this application.
    QSoftMenuBar::setHelpEnabled(this,true);

    //CheckLoadedPictures();

    SetMenuVis(false);
    info1_lbl->setText( "<font color=\"#e0bc70\">Touch the screen to continue</font>" );

    InitState();
    accelerometer_start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerAction()));

    QTimer *acc_timer = new QTimer(this);
    connect(acc_timer, SIGNAL(timeout()), this, SLOT(acc_timerAction()));
    acc_timer->start(PROC_ACC_DATA_INTERVAL);

    installEventFilter(this);

    if (qt_game_config.fullscreen != FULLSCREEN_NONE)
    {
        setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
        setWindowState( windowState() | Qt::WindowFullScreen );
    }

    DisableScreenSaver();

}

/*
 *  Destroys the object and frees any allocated resources
 */
Form::~Form()
{
    // no need to delete child widgets, Qt does it all for us
}
