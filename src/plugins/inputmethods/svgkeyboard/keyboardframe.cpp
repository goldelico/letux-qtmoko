/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2012 Radek Polak <psonek2@seznam.cz>
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
#include <ctype.h>
#include <qtopialog.h>

#include <QDir>
#include <QFile>
#include <QMenu>
#include <Qtopia>
#include <QStyle>
#include <QPalette>
#include <QMouseEvent>
#include <QSoftMenuBar>
#include <QApplication>
#include <QDesktopWidget>

// Add keys from svg file to list. The keys in svg are in form id="key_xxx"
// where xxx is hex value from Qt::Key or unicode value of given key.
static void addKeys(const QString & svgFile, QStringList & keyList)
{
    QFile f(svgFile);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "failed to open kbd file " << svgFile;
        return;
    }
    QByteArray line;
    for (;;) {
        line = f.readLine();
        if (line.isEmpty())
            break;

        int start = line.indexOf("id=\"key_");
        if (start < 0)
            continue;

        int end = line.indexOf('"', start + 8);
        if (end < 0)
            continue;

        QString key = line.mid(start + 8, end - start - 8);
        keyList.append(key);
    }
    f.close();
}

// Return name of the element in svg for given key
static QString elemId(KeyInfo * ki)
{
    return "key_" + QString::number(ki->keycode,
                                    16) + "_" + QString::number(ki->unicode,
                                                                16);
}

// Fill screen resolution independent properties
static bool fillLayout(const QString & svgFile, KeyLayout * layout)
{
    // Make key list
    QStringList keyIds;
    addKeys(svgFile, keyIds);
    int count = layout->numKeys = keyIds.count();
    if (count == 0) {
        qWarning() << "fillLayout: no keys found";
        return false;
    }

    QSvgRenderer *svg = layout->svg = new QSvgRenderer(svgFile);
    KeyInfo *keys = layout->keys =
        (KeyInfo *) (malloc(sizeof(KeyInfo) * count));
    if (keys == NULL) {
        qWarning() << "fillLayout: keys null";
        return false;
    }
    memset((void *)(keys), 0, sizeof(KeyInfo) * count);

    KeyInfo *ki = keys;
    for (int i = 0; i < count; i++) {
        QString id = keyIds.at(i);
        int index = id.indexOf('_');
        if (index < 0) {
            qWarning() << "key " << id << " must be in form scancode_unicode";
            continue;
        }
        QString keycodeStr = id.left(index);
        QString unicodeStr = id.mid(index + 1);
        bool ok;
        ki->keycode = keycodeStr.toInt(&ok, 16);
        if (ok)
            ki->unicode = unicodeStr.toInt(&ok, 16);
        if (!ok)
            qWarning() << "key " << id << ": hex number parse error";

        ki->rectSvg = svg->boundsOnElement(elemId(ki));
        if (i == 0)
            layout->rectSvg = ki->rectSvg;
        else
            layout->rectSvg = layout->rectSvg.united(ki->rectSvg);
        ki++;
    }

    // Order keys by y
    bool ok;
    do {
        ok = true;
        ki = keys;
        for (int i = 1; i < count; i++) {
            KeyInfo *next = ki + 1;
            if (next->rectSvg.top() < ki->rectSvg.top()) {
                ok = false;
                KeyInfo tmp = *next;
                *next = *ki;
                *ki = tmp;
            }
            ki++;
        }
    } while (!ok);

    layout->shifted = svgFile.contains("shift");
    return true;
}

// Compute key positions on the screen
static void placeKeys(KeyLayout * layout, float w, float h)
{
    KeyInfo *ki = layout->keys;
    QRectF lr = layout->rectSvg;    // layout rectangle
    for (int i = 0; i < layout->numKeys; i++) {
        QRectF kr = ki->rectSvg;    // key rectangle on svg
        QRect *ks = &(ki->rectScr); // key rectangle on screen

        ks->setLeft((w * (kr.left() - lr.left())) / lr.width());
        ks->setTop((h * (kr.top() - lr.top())) / lr.height());
        ks->setWidth((w * ki->rectSvg.width()) / lr.width());
        ks->setHeight((h * ki->rectSvg.height()) / lr.height());

        //QPixmap px = ki->pic = QPixmap(ks->width(), ks->height());

        //QPainter p(&px);
        //p.fillRect(*ks, Qt::white);
        //layout->svg->render(&p, elemId(ki), QRectF(0, 0, 48, 48));

        ki++;
    }
}

static void setModifier(Qt::KeyboardModifiers & mods, Qt::KeyboardModifiers mod)
{
    mods |= mod;
}

static void clearModifier(Qt::KeyboardModifiers & mods,
                          Qt::KeyboardModifiers mod)
{
    mods &= ~mod;
}

static void toggleModifier(Qt::KeyboardModifiers & mods,
                           Qt::KeyboardModifiers mod)
{
    if (mods & mod)
        clearModifier(mods, mod);
    else
        setModifier(mods, mod);
}

static Qt::KeyboardModifiers getModifiers(KeyInfo * ki)
{
    switch (ki->keycode) {
    case Qt::Key_Shift:
        return Qt::ShiftModifier;
    case Qt::Key_Alt:
        return Qt::AltModifier;
    case Qt::Key_Control:
        return Qt::ControlModifier;
    default:
        return Qt::NoModifier;
    }
}

static int keyChar(Qt::KeyboardModifiers modifiers, int unicode)
{
    if ((modifiers & Qt::ControlModifier) && unicode >= 'a' && unicode <= 'z')
        return unicode - 'a' + 1;
    return unicode;
}

// Return rectangle for pressed key
static QRect pressedRect(const QRect & keyRect)
{
    QRect res(keyRect);
    res.setTop(res.top() - res.height());
    res.setLeft(keyRect.left() - keyRect.width() / 2);
    res.setRight(keyRect.right() + keyRect.width() / 2);
    return res;
}

KeyboardFrame::KeyboardFrame(QWidget * parent, Qt::WFlags f):
QFrame(parent, f)
    , repeatTimer(this)
    , vib()
    , pressedKey(NULL)
    , highKey(NULL)
    , modifiers(Qt::NoModifier)
    , highTid(0)
    , positionTop(true)
    , caps(1)
    , numLayouts(0)
    , curLayout(0)
{
    setAttribute(Qt::WA_InputMethodTransparent, true);

    setWindowFlags(Qt::Dialog | Qt::
                   WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setFrameStyle(QFrame::Plain | QFrame::Box);

    QPalette pal(palette());
    QColor col(Qt::lightGray);
    col.setAlpha(0);
    pal.setColor(QPalette::Background, col);
    setPalette(pal);
    setAutoFillBackground(true);

    memset((void *)(layouts), 0, sizeof(KeyLayout) * MAX_LAYOUTS);

    connect(&repeatTimer, SIGNAL(timeout()), this, SLOT(repeat()));

    emit needsPositionConfirmation();
}

KeyboardFrame::~KeyboardFrame()
{
    // Free keys - they are malloced
    for (int i = 0; i < numLayouts; i++) {

        KeyInfo *ki = layouts[i].keys;
        while (layouts[i].numKeys--) {
            //ki->pic = NULL;     // TODO: does this free pixmap data?
            ki++;
        }
        free(layouts[i].keys);
    }
}

// Set current layout. The layout is loaded from svg file if was not used yet
void KeyboardFrame::setLayout(int index, bool skipShifted)
{
    if (numLayouts == 0) {
        QDir d(Qtopia::qtopiaDir() + "etc/im/svgkbd");
        QStringList list = d.entryList(QStringList() << "*.svg", QDir::Files);
        list.sort();
        qLog(Input) << "svg kbd layouts in " << d.path() << ": " +
            list.join(", ");
        numLayouts = list.count();

        for (int i = 0; i < numLayouts; i++)
            fillLayout(d.filePath(list.at(i)), &layouts[i]);
    }

    KeyLayout *lay = NULL;
    for(;;)
    {
        if (index >= numLayouts)
            index = 0;
        else if (index < 0)
            index = numLayouts - 1;

        lay = &layouts[index];
        if(lay->shifted && skipShifted)
            index++;
        else
            break;
    }
    
    if (width() != lay->scrWidth || height() != lay->scrHeight) {
        lay->scrWidth = width();
        lay->scrHeight = height();
        placeKeys(lay, lay->scrWidth, lay->scrHeight);
    }

    curLayout = index;
}

void KeyboardFrame::showEvent(QShowEvent * e)
{
    setLayout();
    qwsServer->sendIMQuery(Qt::ImMicroFocus);
    setGeometry(geometryHint());
    placeKeys(&layouts[curLayout], width(), height());
    releaseKeyboard();
    QFrame::showEvent(e);

    // Let keyboardimpl know that we're hidden now, and it should update the
    // menu
    emit showing();
}

void KeyboardFrame::hideEvent(QHideEvent *)
{
    // don't want keypresses to keep going if the widget is hidden, so reset state
    resetState();
    // Let keyboardimpl know that we're hidden now, and it should update the menu
    emit hiding();
};

void KeyboardFrame::resizeEvent(QResizeEvent *)
{
    setLayout();
}

static void renderKey(QPainter * p, QRect clip, QSvgRenderer * svg,
                      KeyInfo * ki)
{
    if (!ki->rectScr.intersects(clip))
        return;

    svg->render(p, elemId(ki), ki->rectScr);

//    if(ki->unicode)
//        p->drawText(ki->rectScr, Qt::AlignCenter, QString(ki->unicode));
//    else
//        svg->render(p, elemId(ki), ki->rectScr);
}

void KeyboardFrame::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.setClipRect(e->rect());
    KeyLayout *lay = &layouts[curLayout];

    // Hide keys when layout key is pressed
    if (pressedKey && pressedKey->keycode == Qt::Key_Mode_switch)
        return;

    // Draw keys - only those that are in clip region
    KeyInfo *ki = lay->keys;
    for (int i = 0; i < lay->numKeys; i++) {
        renderKey(&p, e->rect(), lay->svg, ki);
        ki++;
    }

    // Draw highlighted key - except upper row they are all shifted one row up
    // so that it's not obscured by finger.
    if (highKey) {
        QRect rect = pressedRect(highKey->rectScr);
        if (rect.top() > 0)
            rect.setTop(rect.top() - highKey->rectScr.height());
        lay->svg->render(&p, elemId(highKey), rect);
    }
    //p.setPen(Qt::yellow);
    //p.drawRect(e->rect());
}

void KeyboardFrame::mousePressEvent(QMouseEvent * e)
{
    // Ingore too fast clicks - this filters out some touchscreen errors
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    ignorePress = (1000 * (tp.tv_sec - pressTime.tv_sec) +
                   (tp.tv_nsec - pressTime.tv_nsec) / 1000000) < 100;

    pressTime = tp;
    if (ignorePress)
        return;

    // Clear highlited key if typing fast
    cleanHigh();

    // Vibrate
    vib.setVibrateNow(true, 32);

    // Find pressed key
    KeyInfo *ki = layouts[curLayout].keys;
    int num = layouts[curLayout].numKeys;
    for (;;) {
        if (ki->rectScr.contains(e->x(), e->y()))
            break;

        ki++;
        if (--num <= 0)         // key not found
            return;
    }

    // Handle CAPS lock
    if (ki->keycode == Qt::Key_CapsLock) {
        setLayout(curLayout + caps);
        caps *= -1;
        repaint();
        return;
    }

    pressedKey = ki;

    // Handle layout switch
    if (ki->keycode == Qt::Key_Mode_switch) {
        repaint();
        return;
    }

    Qt::KeyboardModifiers mod = getModifiers(ki);
    if (mod == Qt::NoModifier)
        qwsServer->processKeyEvent(keyChar(modifiers, ki->unicode), ki->keycode,
                                   modifiers, true, false);
    else {
        toggleModifier(modifiers, mod);
        if (mod & Qt::ShiftModifier) {
            if (modifiers & Qt::ShiftModifier)
                setLayout(curLayout + caps);
            else
                setLayout(curLayout - caps);
            repaint();
            return;
        }
    }

    repeatTimer.start(500);

    highKey = ki;
    repaint(pressedRect(ki->rectScr));

    emit needsPositionConfirmation();
}

void KeyboardFrame::mouseReleaseEvent(QMouseEvent *e)
{
    if (ignorePress)
        return;

    if (pressedKey) {
        if (pressedKey->keycode == Qt::Key_Mode_switch) {
            
            // Switch layout if released on the same rect
            if(pressedKey->rectScr.contains(e->pos())) {
                caps = false;
                setLayout(curLayout + 1, true);
            }
            pressedKey = NULL;
            repaint();
            return;
        }

        qwsServer->processKeyEvent(keyChar(modifiers, pressedKey->unicode),
                                   pressedKey->keycode, modifiers, false,
                                   false);

        // Clear shift and modifiers after regular key
        if (getModifiers(pressedKey) == Qt::NoModifier) {
            if (modifiers & Qt::ShiftModifier) {
                setLayout(curLayout - caps);    // clear shift
                highKey = NULL;
                repaint();
            }
            modifiers = Qt::NoModifier; // clear modifiers
        }
    }
    // This hides highlighted key after 200ms, condition should be always true
    if (highTid == 0)
        highTid = startTimer(200);

    repeatTimer.stop();
}

// Clean highlighted key
void KeyboardFrame::cleanHigh()
{
    if (highTid) {
        killTimer(highTid);
        highTid = 0;
    }

    if (highKey) {
        QRect rect = pressedRect(highKey->rectScr);
        highKey = NULL;
        repaint(rect);
    }
}

void KeyboardFrame::timerEvent(QTimerEvent * e)
{
    if (e->timerId() == highTid) {
        cleanHigh();
    }
}

void KeyboardFrame::repeat()
{
    if (pressedKey && pressedKey->unicode > 0) {
        repeatTimer.start(150);
        qwsServer->processKeyEvent(keyChar(modifiers, pressedKey->unicode),
                                   pressedKey->keycode, modifiers, true, true);
    } else
        repeatTimer.stop();
}

QRect KeyboardFrame::geometryHint() const
{
    QRect r = QApplication::desktop()->availableGeometry();
    if (r.width() <= 480)
        r.setHeight(480);       // hack to make it larger
    return r;
}

QSize KeyboardFrame::sizeHint() const
{
    return geometryHint().size();
}

void KeyboardFrame::resetState()
{
    pressedKey = NULL;
    modifiers = Qt::NoModifier;
    repeatTimer.stop();
}

bool KeyboardFrame::obscures(const QPoint & point)
{
    QRect mwr = QApplication::desktop()->availableGeometry();
    bool isTop = point.y() < (mwr.y() + (mwr.height() >> 1));
    return (isTop == positionTop);
}

void KeyboardFrame::swapPosition()
{
    QRect mwr = QApplication::desktop()->availableGeometry();
    if (positionTop) {
        move(mwr.bottomLeft() - QPoint(0, height()));
        positionTop = false;
    } else {
        move(mwr.topLeft());
        positionTop = true;
    }
}
