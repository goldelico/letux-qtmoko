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

#ifndef KEYBOARDFRAME_H
#define KEYBOARDFRAME_H

#include <time.h>
#include <QDebug>
#include <QVibrateAccessory>
#include <QSvgRenderer>
#include <QPixmap>
#include <QFrame>

#define MAX_LAYOUTS 5

class QTimer;

enum currentPosition
{
    Top,
    Bottom
};

struct KeyInfo {
    int qcode;          // value from Qt::Key or unicode value
    QRectF rectSvg;     // bounding rectangle in SVG
    QRectF rectScr;     // bounding rectangle on screen
    QPixmap pic;
};

struct KeyLayout
{
    int numKeys;
    KeyInfo *keys;      // pointer to first key info
    QSvgRenderer *svg;
    QRectF rectSvg;
};

/*
    KeyboardFrame is the primary widget for the Keyboard inputmethod.
    It is responsible for marshalling pickboards for displaying the pickboard,
    and for handling mouseevents.

    It currently also creates and dispatches keyevents, although this is
    expected to be re-routed through Keyboard in the future.
*/
class KeyboardFrame : public QFrame
{
    Q_OBJECT
public:
    explicit KeyboardFrame( QWidget* parent=0, Qt::WFlags f=0 );
    virtual ~KeyboardFrame();

    void resetState();

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    void showEvent(QShowEvent*);
    void paintEvent(QPaintEvent* e);
    void timerEvent(QTimerEvent* e);
    void drawKeyboard(QPainter &p, const QRect& clip, int key = -1);

    void hideEvent ( QHideEvent * );

    QRect geometryHint() const;
    QSize sizeHint() const;

    bool obscures( const QPoint &point );

    void focusInEvent ( QFocusEvent *e){
    Q_UNUSED(e);
    qWarning() << "Warning: keyboard got focus";
    };

    bool filter(int /*unicode*/, int /*keycode*/, int /*modifiers*/,
                        bool /*isPress*/, bool /*autoRepeat*/){return false;};

    bool filter(const QPoint &, int /*state*/, int /*wheel*/){return false;};

signals:
    void needsPositionConfirmation();
    void hiding();
    void showing();

public slots:
    void swapPosition();

protected:
//    void reset(){QWSInputMethod::reset();};
//    void updateHandler(int type){};
//    void mouseHandler(int pos, int state){};
//    void queryResponse(int property, const QVariant&){};


private slots:
    void repeat();

private:
    KeyInfo *pressedKey;            // currently pressed key or NULL
    int pressedChar;                // unicode value of pressed key
    Qt::KeyboardModifiers modifiers;

    int pressTid;
    struct timespec pressTime;      // last time key was pressed
    bool ignorePress;               // used to ignore too fast presses

    QVibrateAccessory vib;

    bool microFocusPending;
    bool showPending;

    bool positionTop;

    QTimer *repeatTimer;
    
    int numLayouts;                 // number of currently loaded layouts
    int curLayout;                  // current layout
    KeyLayout layouts[MAX_LAYOUTS];
};

#endif
