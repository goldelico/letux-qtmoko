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

#include <QPaintEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QFile>
#include <QStyle>
#include <QSoftMenuBar>

#include <qtopialog.h>

#define USE_SMALL_BACKSPACE

enum { BSCode = 0x80, TabCode, CapsCode, RetCode,
       ShiftCode, CtrlCode, AltCode, SpaceCode, BackSlash,
       UpCode, LeftCode, DownCode, RightCode, Blank, Expand,
       Opti, ResetDict,
       Divide, Multiply, Add, Subtract, Decimal, Equal,
       Percent, Sqrt, Inverse, Escape };

typedef struct SpecialMap {
    int qcode;
    ushort unicode;
    const char * label;
};

static SpecialMap specialM[] = {
    {   Qt::Key_Backspace,      8,      "bsp"},
    {   Qt::Key_Tab,            9,      "tab"},
    {   Qt::Key_CapsLock,       0xffff, "cap"},
    {   Qt::Key_Return,         13,     "ret"},
    {   Qt::Key_Shift,          0xffff, "shi"},
    {   Qt::Key_Control,        0xffff, "ctr"},
    {   Qt::Key_Alt,            0xffff, "alt"},
    {   Qt::Key_Space,          ' ',    "spa"},
    {   BackSlash,              43,     "bsl"},
    {   Qt::Key_Up,             0xffff, "up"},
    {   Qt::Key_Left,           0xffff, "lef"},
    {   Qt::Key_Down,           0xffff, "dow"},
    {   Qt::Key_Right,          0xffff, "rig"},
    {   Qt::Key_Insert,         0xffff, "ins"},
    {   Qt::Key_Home,           0xffff, "hom"},
    {   Qt::Key_PageUp,         0xffff, "pgu"},
    {   Qt::Key_End,            0xffff, "pgd"},
    {   Qt::Key_Delete,         0xffff, "del"},
    {   Qt::Key_PageDown,       0xffff, "pgd"},
    {   Blank,                  0,      " "},
    {   Expand,                 0xffff, "exp"},
    {   Opti,                   0xffff, "opt"},
    // number pad stuff
    {   Divide,                 0,      "div"},
    {   Multiply,               0,      "mul"},
    {   Add,                    0,      "add"},
    {   Subtract,               0,      "sub"},
    {   Decimal,                0,      "dec"},
    {   Equal,                  0,      "eq"},
    {   Percent,                0,      "per"},
    {   Sqrt,                   0,      "sqr"},
    {   Inverse,                0,      "inv"},
    {   Escape,                 27,     "esc"},
    {   0,                      0,      NULL}
};

// Add keys from svg file to list. The keys are in form id="key_a" where a is
// letter or can be number (key code).
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
        qDebug() << "key=" << key;
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

KeyboardFrame::KeyboardFrame(QWidget* parent, Qt::WFlags f) :
    QFrame(parent, f), shift(false), lock(false), ctrl(false),
    alt(false), useLargeKeys(true), useOptiKeys(0), pressedKey(-1),
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

    picks = new KeyboardPicks( this );
    picks->initialise();

    curLayout = 0;
    numLayouts = 1;
    fillLayout("/qwerty.svg", &layouts[0]);
    placeKeys(&layouts[0], width(), height());

    QColor backcolor = palette().shadow().color();
    backcolor.setAlpha(196);

    repeatTimer = new QTimer( this );
    connect( repeatTimer, SIGNAL(timeout()), this, SLOT(repeat()) );

    emit needsPositionConfirmation();
}

KeyboardFrame::~KeyboardFrame()
{
}

void KeyboardFrame::showEvent(QShowEvent *e)
{
    qwsServer->sendIMQuery ( Qt::ImMicroFocus );
    setGeometry(geometryHint());
    releaseKeyboard();
    int ph = picks->sizeHint().height();

//    picks->setGeometry( 0, 0, width(), ph );
//    move(0,mwr.height()-height());

    keyHeight = (height()-ph)/5;
    int nk;
    if ( useOptiKeys ) {
        nk = 7;
    } else if ( useLargeKeys ) {
        nk = 15;
    } else {
        nk = 19;
    }
    
    defaultKeyWidth = width()/nk;
    xoffs = (width()-defaultKeyWidth*nk)/2;
    QFrame::showEvent(e);
    // Let keyboardimpl know that we're hidden now, and it should update the menu
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
    int ph = picks->sizeHint().height();
//    picks->setGeometry( 0, 50, width(), ph );
    keyHeight = (height()-ph)/5;
    int nk;
    if ( useOptiKeys ) {
        nk = 7;
    } else if ( useLargeKeys ) {
        nk = 15;
    } else {
        nk = 19;
    }
    defaultKeyWidth = width()/nk;
    xoffs = (width()-defaultKeyWidth*nk)/2;
    
    placeKeys(&layouts[curLayout], width(), height());
}


KeyboardPicks::~KeyboardPicks()
{
    delete dc;
}

void KeyboardPicks::initialise()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    mode = 0;
    dc = new KeyboardConfig(this);
    configs.append(dc);
}

QSize KeyboardPicks::sizeHint() const
{
    //return QSize(240,fontMetrics().lineSpacing());    // <-- uncomment to enable text hints (quite useless for this type of keyboard)
    return QSize(240, 0);
}


void  KeyboardConfig::generateText(const QString &s)
{
#if defined(Q_WS_QWS) || defined(Q_WS_QWS)
                  int i;
    for ( i=0; i<(int)backspaces; i++) {
        qwsServer->processKeyEvent( 8, Qt::Key_Backspace, 0, true, false );
        qwsServer->processKeyEvent( 8, Qt::Key_Backspace, 0, false, false );
    }
    for ( i=0; i<(int)s.length(); i++) {
        uint code = 0;
        if ( s[i].unicode() >= 'a' && s[i].unicode() <= 'z' ) {
            code = s[i].unicode() - 'a' + Qt::Key_A;
        }
        qwsServer->processKeyEvent( s[i].unicode(), code, 0, true, false );
        qwsServer->processKeyEvent( s[i].unicode(), code, 0, false, false );
    }
    qwsServer->processKeyEvent( ' ', Qt::Key_Space, 0, true, false );
    qwsServer->processKeyEvent( ' ', Qt::Key_Space, 0, false, false );
    backspaces = 0;
#endif
}


//PC keyboard layout and scancodes

/*
  Format: length, code, length, code, ..., 0

  length is measured in half the width of a standard key.
  If code < 0x80, code gives the ASCII value of the key

  If code >= 0x80, the key is looked up in specialM[].

 */

/* Keyboard layout shamelessly stolen from SHR

Esc 1 2 3 4 5 6 7 8 9 0 - = <---
TABq w e r t y u i o p [ ] \ Del
CAP a s d f g h j k l ; ' <=====
SHIF z x c v b n m , . / `   Ins
>  CtrlAltSPACE < > ^ _ PuPdHoEn

*/
static const uchar * const keyboard_standard[5] = {
    (const uchar *const)"\002\240\0021\0022\0023\0024\0025\0026\0027\0028\0029\0020\002-\002=\004\200",
    (const uchar *const)"\003\201\002q\002w\002e\002r\002t\002y\002u\002i\002o\002p\002[\002]\002\\\003\221",
    (const uchar *const)"\004\202\002a\002s\002d\002f\002g\002h\002j\002k\002l\002;\002'\004\203",
    (const uchar *const)"\005\204\002z\002x\002c\002v\002b\002n\002m\002,\002.\002/\002`\002\215",
    (const uchar *const)"\004\225\003\205\002\206\005\207\002\212\002\214\002\211\002\213\002\217\002\222\002\216\002\220"
};


/* Keyboard layout for SMS/mails

1 2 3 4 5 6 7 8 9 0
q w e r t y u i o p
a s d f g h j k l <===
SHIF z x c v b n m , .
> SPACE < > ^ _    <--

*/
static const uchar * const keyboard_opti[5] = {
    (const uchar *const)"\002a\002b\002c\002d\002e\002f\004\200",
    (const uchar *const)"\002g\002h\002i\002j\002k\002l\004\200",
    (const uchar *const)"\002m\002n\002o\002p\002q\002r\004\203",
    (const uchar *const)"\002s\002t\002u\002v\002w\002x\004\203",
    (const uchar *const)"\002\225\006\207\002y\002z\002,\002."
};


struct ShiftMap {
    char normal;
    char shifted;
};

enum modsIndex {
    ShiftMod = 0,
    AltMod = 1,
    CtrlMod = 2
};

struct keyLocation {
    int x, y;
};

static const keyLocation modsLocMap[] = {
  {0, 3}, {1, 4}, {0, 4}
};


static const ShiftMap shiftMap[] = {
    { '`', '~' },
    { '1', '!' },
    { '2', '@' },
    { '3', '#' },
    { '4', '$' },
    { '5', '%' },
    { '6', '^' },
    { '7', '&' },
    { '8', '*' },
    { '9', '(' },
    { '0', ')' },
    { '-', '_' },
    { '=', '+' },
    { '\\', '|' },
    { '[', '{' },
    { ']', '}' },
    { ';', ':' },
    { '\'', '"' },
    { ',', '<' },
    { '.', '>' },
    { '/', '?' }
};


int KeyboardFrame::keycode( int i2, int j, const uchar **keyboard, QRect *repaintrect )
{
    if ( j <0 || j >= 5 )
        return 0;

    const uchar *row = keyboard[j];

    int x = 0;
    while ( *row && x+*row <= i2 ) {
        x += *row;
        row += 2;
    }

    if ( !*row ) return 0;

    if ( repaintrect ) {
        *repaintrect = QRect(
            x*defaultKeyWidth/2+xoffs,
            j*keyHeight+picks->height(),
            *row*defaultKeyWidth/2,keyHeight);
    }

    int k;
    if ( row[1] >= 0x80 ) {
        k = row[1];
    } else {
        k = row[1]+(i2-x)/2;
    }

    return k;
}


/*
  return scancode and width of first key in row \a j if \a j >= 0,
  or next key on current row if \a j < 0.

*/

int KeyboardFrame::getKey( int &w, int j ) {
    static const uchar *row = 0;
    static int key_i = 0;
    static int scancode = 0;
    static int half = 0;

    if ( j >= 0 && j < 5 ) {
        if (useOptiKeys)
            row = keyboard_opti[j];
        else
            row = keyboard_standard[j];
        half=0;
    }

    if ( !row || !*row ) {
        return 0;
    } else if ( row[1] >= 0x80 ) {
        scancode = row[1];
        w = (row[0] * w + (half++&1)) / 2;
        row += 2;
        return scancode;
    } else if ( key_i <= 0 ) {
        key_i = row[0]/2;
        scancode = row[1];
    }
    key_i--;
    if ( key_i <= 0 )
        row += 2;
    return scancode++;
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


/*
  Draw the KeyboardFrame.

  If key >= 0, only the specified key is drawn.
*/
void KeyboardFrame::drawKeyboard( QPainter &p, const QRect& clip, int key )
{
    QColor keycolor_pressed = palette().mid().color();
    QColor backcolor = palette().shadow().color();
    QColor textcolor = palette().light().color();
    
    backcolor.setAlpha(196);    
    p.fillRect(clip, backcolor);

    for ( int j = 0; j < 5; j++ ) {
        int y = j * keyHeight + picks->height() + 1;
        if ( y <= clip.bottom() && y+keyHeight >= clip.top() ) {
            int x = xoffs;
            int kw = defaultKeyWidth;
            int k= getKey( kw, j );
            while ( k ) {
                if ( (key < 0 || k == key) && x <= clip.right() && x+kw > clip.left() ) {
                    QString s;
                    bool pressed = (k == pressedKey);
                    bool blank = (k == 0223);

                    if ( k >= 0x80 ) {
                        s = specialM[k - 0x80].label;

                        if ( k == ShiftCode ) {
                            pressed = shift;
                        } else if ( k == CapsCode ) {
                            pressed = lock;
                        } else if ( k == CtrlCode ) {
                            pressed = ctrl;
                        } else if ( k == AltCode ) {
                            pressed = alt;
                        }
                    } else {
    #if defined(Q_WS_QWS) || defined(Q_WS_QWS)
    /*
                        s = QChar( shift^lock ? QWSServer::keyMap()[k].shift_unicode :
                                   QWSServer::keyMap()[k].unicode);
    */
                        // ### Fixme, bad code, needs improving, whole thing needs to
                        // be re-coded to get rid of the way it did things with scancodes etc
                        char shifted = k;
                        if ( !isalpha( k ) ) {
                            for ( unsigned i = 0; i < sizeof(shiftMap)/sizeof(ShiftMap); i++ )
                                if ( shiftMap[i].normal == k )
                                    shifted = shiftMap[i].shifted;
                        } else {
                            shifted = toupper( k );
                        }
                        s = QChar( shift^lock ? shifted : k );
    #endif
                    }

                    if (!blank) {
                        if ( pressed )
                            p.fillRect( x, y, kw, keyHeight-1, keycolor_pressed );

                            p.setFont(font());
                            p.setPen(textcolor);
                            if(pressed)
                                p.drawText( x - 1, y, kw, keyHeight-2, Qt::AlignTop | Qt::AlignHCenter, s );
                            else
                                p.drawText( x - 1, y, kw, keyHeight-2, Qt::AlignCenter, s );
                    }
                }

                x += kw;
                kw = defaultKeyWidth;
                k = getKey( kw );
            }
        }
    }
}


void KeyboardFrame::mousePressEvent(QMouseEvent *e)
{
    clearHighlight(); // typing fast?

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    ignorePress = (1000 * (tp.tv_sec - pressTime.tv_sec) +
        (tp.tv_nsec - pressTime.tv_nsec) / 1000000) < 100;

    pressTime = tp;
    if(ignorePress)
        return;

    vib.setVibrateNow(true, 32);

    KeyInfo *ki = layouts[curLayout].keys;
    int num = layouts[curLayout].numKeys;
    for(;;)
    {
        if(ki->rectScr.contains(e->x(), e->y()))
        {
            qDebug() << "pressedKi=" << ki->qcode;
            break;
        }
        ki++;
        if(--num <= 0)      // key not found
            return;
    }
    
    
    int i2 = ((e->x() - xoffs) * 2) / defaultKeyWidth;
    int j = (e->y() - picks->height()) / keyHeight;

    QRect keyrect;
    //int k = keycode( i2, j, (const uchar **)((useOptiKeys) ? keyboard_opti : keyboard_standard), &keyrect );
    
    int k = ki->qcode;

    bool key_down = false;
    unicode = -1;
    qkeycode = 0;
    if ( k >= 0x80 ) {
        if ( k == Qt::Key_Shift ) {
            shift = !shift;
            keyrect = rect();
        } else if ( k == AltCode ){
            alt = !alt;
        } else if ( k == CapsCode ) {
            lock = !lock;
            keyrect = rect();
        } else if ( k == CtrlCode ) {
            ctrl = !ctrl;
        } else if ( k == 0224 /* Expand */ ) {
            useLargeKeys = !useLargeKeys;
            resizeEvent(0);
            repaint( ); // need it to clear first
        } else if ( k == 0225 /* Opti/Toggle */ ) {
            useOptiKeys = !useOptiKeys;
            resizeEvent(0);
            repaint( ); // need it to clear first
        } else {
            qkeycode = specialM[ k - 0x80 ].qcode;
            unicode = specialM[ k - 0x80 ].unicode;
        }
    } else {
        //due to the way the keyboard is defined, we know that
        //k is within the ASCII range, and can be directly mapped to
        //a qkeycode; except letters, which are all uppercase
        qkeycode = toupper(k);
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
        }
    }
    if  ( unicode != -1 ) {
        if ( ctrl && unicode >= 'a' && unicode <= 'z' )
            unicode = unicode - 'a'+1;

        modifiers = Qt::NoModifier;
        if (shift) {
            modifiers |= Qt::ShiftModifier;
            keyrect = rect();
        }
        if (ctrl)
            modifiers |= Qt::ControlModifier;
        if (alt)
            modifiers |= Qt::AltModifier;

        qLog(Input) << "keypressed: code=" << unicode;

        qwsServer->processKeyEvent( unicode, qkeycode, modifiers, true, false );

	clearMods();

        KeyboardConfig *dc = picks->dc;

        if (dc) {
            if (qkeycode == Qt::Key_Backspace) {
                if (dc->input.count()) {
                    dc->input.removeLast();
                    dc->decBackspaces();
                }
            } else if ( k == 0226 || qkeycode == Qt::Key_Return ||
                        qkeycode == Qt::Key_Space ||
                        QChar(unicode).isPunct() ) {
                dc->input.clear();
                dc->resetBackspaces();
            } else {
                dc->add(QString(QChar(unicode)));
                dc->incBackspaces();
            }
        }

        picks->repaint();

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
    
    //vib.setVibrateNow(false);

    repeatTimer->stop();
    if ( pressTid == 0 )
        clearHighlight();
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
        if ( !pressed )
            clearHighlight();
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

void KeyboardFrame::clearMod(int modindex) {
    const keyLocation* loc = &modsLocMap[modindex];

    const uchar **keyboard = (const uchar **)((useOptiKeys) ? keyboard_opti : keyboard_standard);

    const uchar *row = keyboard[loc->y];

    int x = 0;

    for ( int id = 0; id != loc->x; row += 2, id++ )
	x += *row;

    QRect toRepaint = QRect(
	x*defaultKeyWidth/2+xoffs,
        loc->y*keyHeight+picks->height(),
        *row*defaultKeyWidth/2,keyHeight
    );

    repaint (toRepaint);
}

void KeyboardFrame::clearMods()
{
    if (shift) {
	shift = false;
	clearMod(ShiftMod);

    }
    if (alt) {
	alt = false;
	clearMod(AltMod);
    }
    if (ctrl) {
	ctrl = false;
	clearMod(CtrlMod);
    }
}

void KeyboardFrame::clearHighlight()
{
    if ( pressedKey >= 0 ) {
        pressedKey = -1;
        repaint(pressedKeyRect);
    }
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
    picks->resetState();
    shift = false;
    lock = false;
    ctrl = false;
    alt = false;
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
