/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#include "keyboardframe.h"

#ifdef Q_WS_QWS
#include <qwindowsystem_qws.h>
#endif
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qtimer.h>
#include <ctype.h>
#include <qtopialog.h>

#include <QPaintEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QFile>
#include <QStyle>
#include <QSoftMenuBar>

// Add keys from svg file to list. The keys in svg are in form id="key_xxx"
// where xxx is hex value from Qt::Key or unicode value of given key.
static void addKeys(const QString & svgFile, QStringList & keyList)
{
    QFile f(svgFile);
    if(!f.open(QIODevice::ReadOnly))
    {
        qWarning() << "failed to open kbd file " << svgFile;
        return;
    }
    QByteArray line;
    for(;;)
    {
        line = f.readLine();
        if(line.isEmpty())
            break;

        int start = line.indexOf("id=\"key_");
        if(start < 0)
            continue;

        int end = line.indexOf('"', start + 8);
        if(end < 0)
            continue;

        QString key = line.mid(start + 8, end - start - 8);
        keyList.append(key);
    }
    f.close();
}

// Return name of the element in svg for given key
static QString elemId(KeyInfo *ki)
{
    return "key_" + QString::number(ki->qcode, 16);
}

// Fill screen resolution independent properties
static bool fillLayout(const QString & svgFile, KeyLayout *layout)
{
    // Make key list
    QStringList keyIds;
    addKeys(svgFile, keyIds);
    int count = layout->numKeys = keyIds.count();
    if(count == 0) {
        qWarning() << "fillLayout: no keys found";
        return false;
    }

    QSvgRenderer *svg = layout->svg = new QSvgRenderer(svgFile);
    KeyInfo *keys = layout->keys = (KeyInfo *)(malloc(sizeof(KeyInfo) * count));
    if(keys == NULL) {
        qWarning() << "fillLayout: keys null";
        return false;
    }
    memset((void*)(keys), 0, sizeof(KeyInfo) * count);

    KeyInfo *ki = keys;
    for(int i = 0; i < keyIds.count(); i++)
    {
        QString id = keyIds.at(i);
        bool ok;
        ki->qcode = id.toInt(&ok, 16);
        if(!ok)
            qWarning() << "key id=" << id << " must be hex number";
        
        ki->rectSvg = svg->boundsOnElement(elemId(ki));
        if(i == 0)
            layout->rectSvg = ki->rectSvg;
        else
            layout->rectSvg = layout->rectSvg.united(ki->rectSvg);
        ki++;
    }
    return true;
}

// Compute key positions on the screen
static void placeKeys(KeyLayout *layout, float w, float h)
{
    KeyInfo *ki = layout->keys;
    QRectF lr = layout->rectSvg;    // layout rectangle
    for(int i = 0; i < layout->numKeys; i++)
    {
        QRectF kr = ki->rectSvg;        // key rectangle on svg
        QRectF *ks = &(ki->rectScr);       // key rectangle on screen

        ks->setLeft((w * (kr.left() - lr.left())) / lr.width());
        ks->setTop((h * (kr.top() - lr.top())) / lr.height());
        ks->setWidth((w * ki->rectSvg.width()) / lr.width());
        ks->setHeight((h * ki->rectSvg.height()) / lr.height());

        QPixmap px = ki->pic = QPixmap(ks->width(), ks->height());

        QPainter p(&px);
        p.fillRect(*ks, Qt::white);
        layout->svg->render(&p, elemId(ki), QRectF(0, 0, 48, 48));

        ki++;
    }
}

// Return ASCII/unicode value for given KeyInfo. For special keys like shift,
// ctrl and alt return -1
static int getKeyChar(KeyInfo *ki)
{
    switch(ki->qcode)
    {
        case Qt::Key_Backspace: return 8;
        case Qt::Key_Tab: return 9;
        case Qt::Key_Return: return 13;
        case Qt::Key_Escape: return 27;
        case Qt::Key_Shift:
        case Qt::Key_Alt:
        case Qt::Key_Control:
        case Qt::Key_Mode_switch: return -1;
        default: return ki->qcode;
    }
}

static void setModifier(Qt::KeyboardModifiers & mods, Qt::KeyboardModifiers mod)
{
    mods |= mod;
}

static void clearModifier(Qt::KeyboardModifiers & mods, Qt::KeyboardModifiers mod)
{
    mods &= ~mod;
}

static void toggleModifier(Qt::KeyboardModifiers & mods, Qt::KeyboardModifiers mod)
{
    if(mods & mod)
        clearModifier(mods, mod);
    else
        setModifier(mods, mod);
}

KeyboardFrame::KeyboardFrame(QWidget* parent, Qt::WFlags f) :
    QFrame(parent, f), pressedKey(-1),
    unicode(-1), qkeycode(0), modifiers(Qt::NoModifier), pressTid(0), pressed(false),
    vib(), positionTop(true), pixS(64, 64)
{
    setAttribute(Qt::WA_InputMethodTransparent, true);

    setPalette(QPalette(QColor(220,220,220))); // Gray
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setFrameStyle(QFrame::Plain | QFrame::Box);

    QRect mwr = QApplication::desktop()->availableGeometry();
    qreal pixHeight = mwr.height()/32;
    qreal pointHeight = (pixHeight*72)/logicalDpiY();
    
    QFont fnt = QApplication::font();
    fnt.setPointSizeF(pointHeight);
    
    QPalette pal(palette());
    QColor col(Qt::lightGray);
    col.setAlpha(0);
    pal.setColor(QPalette::Background, col);
    setPalette(pal);
    setAutoFillBackground(true);

    curLayout = 0;
    numLayouts = 1;
    fillLayout("/qwerty.svg", &layouts[0]);

    QColor backcolor = palette().shadow().color();
    backcolor.setAlpha(196);

    repeatTimer = new QTimer( this );
    connect( repeatTimer, SIGNAL(timeout()), this, SLOT(repeat()) );

    emit needsPositionConfirmation();
}

KeyboardFrame::~KeyboardFrame()
{
    // Free keys - they are malloced
    for(int i = 0; i < numLayouts; i++) {
        
        KeyInfo *ki = layouts[i].keys;
        while(layouts[i].numKeys--)
        {
            ki->pic = NULL;     // TODO: does this free pixmap data?
            ki++;
        }
        free(layouts[i].keys);
    }
}

void KeyboardFrame::showEvent(QShowEvent *e)
{
    qwsServer->sendIMQuery ( Qt::ImMicroFocus );
    setGeometry(geometryHint());
    placeKeys(&layouts[curLayout], width(), height());
    releaseKeyboard();
    QFrame::showEvent(e);

    // Let keyboardimpl know that we're hidden now, and it should update the
    // menu
    emit showing();
}

void KeyboardFrame::hideEvent( QHideEvent * )
{
    // don't want keypresses to keep going if the widget is hidden, so reset state
    resetState();
    // Let keyboardimpl know that we're hidden now, and it should update the menu
    emit hiding();
};


void KeyboardFrame::resizeEvent(QResizeEvent*)
{
    placeKeys(&layouts[curLayout], width(), height());
}

void KeyboardFrame::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    
    KeyInfo *ki = layouts[0].keys;
    for(int i = 0; i < layouts[curLayout].numKeys; i++)
    {
//        qDebug() << "pressedKey=" << pressedKey << ", ki->qcode=" << ki->qcode;
        if(pressedKey >= 0 && pressedKey != ki->qcode)
            continue;
        
        QRectF kr = ki->rectSvg;        // key rectangle on svg
        layouts[0].svg->render(&p, elemId(ki), ki->rectScr);

        ki++;
    }
    
    p.drawLine(0, 0, 100, 100);
/*    p.drawPixmap(0, 0, pixS);
    
    QSvgRenderer svg(QString("/qwerty.svg"));
    svg.render(&p, "R", QRectF(100, 100, 48, 48));
    svg.render(&p, "test", QRectF(150, 150, 48, 48));
    svg.render(&p, "key_a", QRectF(150, 200, 48, 48));
    svg.render(&p, "key_s", QRectF(200, 200, 48, 48));
    svg.render(&p, "key_d", QRectF(250, 200, 48, 48));
    svg.render(&p, "key_f", QRectF(300, 200, 48, 48));
    svg.render(&p, "key_g", QRectF(350, 200, 48, 48));*/
    
    //painter.setClipRect(e->rect());
    //drawKeyboard( painter, e->rect() );
    //picks->dc->draw( &painter );
}

void KeyboardFrame::mousePressEvent(QMouseEvent *e)
{
    // Ingore too fast clicks - this filters out some touchscreen errors
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    ignorePress = (1000 * (tp.tv_sec - pressTime.tv_sec) +
        (tp.tv_nsec - pressTime.tv_nsec) / 1000000) < 100;

    pressTime = tp;
    if(ignorePress)
        return;

    // Vibrate
    vib.setVibrateNow(true, 32);

    // Find pressed key
    KeyInfo *ki = layouts[curLayout].keys;
    int num = layouts[curLayout].numKeys;
    for(;;)
    {
        if(ki->rectScr.contains(e->x(), e->y()))
        {
            qLog(Input) << "pressedKi=" << ki->qcode;
            break;
        }
        ki++;
        if(--num <= 0)      // key not found
            return;
    }

    QRect keyrect;
    int k = ki->qcode;

    bool key_down = false;
    unicode = getKeyChar(ki);
    qkeycode = k;
    if ( unicode == -1 ) {
        
        switch(k)
        {
            case Qt::Key_Shift:
                toggleModifier(modifiers, Qt::ShiftModifier);
                break;
                
            case Qt::Key_Alt:
                toggleModifier(modifiers, Qt::AltModifier);
                break;
                
            case Qt::Key_Control:
                toggleModifier(modifiers, Qt::ControlModifier);
                unicode = -1;
                break;
                
            case Qt::Key_Mode_switch:
                break;
            
        }
    } else {
        //due to the way the keyboard is defined, we know that
        //k is within the ASCII range, and can be directly mapped to
        //a qkeycode; except letters, which are all uppercase
        /*qkeycode = toupper(k);
        if ( shift^lock ) {
            if ( !isalpha( k ) ) {
            for ( unsigned i = 0; i < sizeof(shiftMap)/sizeof(ShiftMap); i++ )
                if ( shiftMap[i].normal == k ) {
                    unicode = shiftMap[i].shifted;
                    qkeycode = unicode;
                    break;
                }
            } else {
                unicode = toupper( k );
            }
        } else {
            unicode = k;
        }*/
    }
    if  ( unicode != -1 ) {
        if ( (modifiers & Qt::ControlModifier) && unicode >= 'a' && unicode <= 'z' )
            unicode = unicode - 'a'+1;

        qLog(Input) << "keypressed: code=" << unicode;

        qwsServer->processKeyEvent( unicode, qkeycode, modifiers, true, false );

        modifiers = Qt::NoModifier;

        key_down = true;
    }
    pressedKey = k;
    qDebug() << "pressedKey=" << pressedKey;
    pressedKeyRect = keyrect;
    repaint(keyrect);
    if ( pressTid )
        killTimer(pressTid);
    pressTid = startTimer(80);
    pressed = true;
    if(key_down)
    {
        repeatTimer->start( 500 );
    };
    emit needsPositionConfirmation();
}


void KeyboardFrame::mouseReleaseEvent(QMouseEvent*)
{
    if(ignorePress)
        return;

    repeatTimer->stop();
    if ( pressTid == 0 )
#if defined(Q_WS_QWS) || defined(Q_WS_QWS)
    if ( unicode != -1 || qkeycode != 0) {
        qLog(Input) << "keyrelease: code=" << unicode;
        qwsServer->processKeyEvent( unicode, qkeycode, modifiers, false, false );
    }
#endif
    pressed = false;
}

void KeyboardFrame::timerEvent(QTimerEvent* e)
{
    if ( e->timerId() == pressTid ) {
        killTimer(pressTid);
        pressTid = 0;
    }
}

void KeyboardFrame::repeat()
{
    if ( pressed && (unicode != -1 || qkeycode != 0)) {
        repeatTimer->start( 150 );
        qwsServer->processKeyEvent( unicode, qkeycode, modifiers, true, true );
    } else
        repeatTimer->stop();
}

QRect KeyboardFrame::geometryHint() const
{
    QRect r = QApplication::desktop()->availableGeometry();
    if(r.width() <= 480)
        r.setHeight(480);  // hack - make it larger

    int sb = style()->pixelMetric(QStyle::PM_ScrollBarExtent) +
             style()->pixelMetric(QStyle::PM_LayoutRightMargin);
    r.setWidth(r.width() - sb);
    return r;
}

QSize KeyboardFrame::sizeHint() const
{
    return geometryHint().size();
}

void KeyboardFrame::resetState()
{
    pressedKey = -1;
    unicode = -1;
    qkeycode = 0;
    modifiers = Qt::NoModifier;
    pressed = false;
    if ( pressTid ) {
        killTimer(pressTid);
        pressTid = 0;
    };
    repeatTimer->stop();

}


bool KeyboardFrame::obscures(const QPoint &point)
{
    QRect mwr = QApplication::desktop()->availableGeometry();
    bool isTop = point.y() < (mwr.y()+mwr.height()>>1);
    return (isTop == positionTop);
}

void KeyboardFrame::swapPosition()
{
    QRect mwr = QApplication::desktop()->availableGeometry();
    if(positionTop)
    {
        move(mwr.bottomLeft()-QPoint(0,height()));
        positionTop = false;
    } else
    {
        move(mwr.topLeft());
        positionTop = true;
    }
}
