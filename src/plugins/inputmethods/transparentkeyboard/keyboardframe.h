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

#ifndef KEYBOARDFRAME_H
#define KEYBOARDFRAME_H

#include <time.h>
#include <QDebug>
#include <QVibrateAccessory>
#include <QSvgRenderer>
#include <QPixmap>
#include <QFrame>
#include <QTimer>

#define MAX_LAYOUTS 5

struct KeyInfo
{
    int keycode;                // value from Qt::Key enum
    int unicode;                // unicode value
    QRectF rectSvg;             // bounding rectangle in SVG
    QRect rectScr;              // bounding rectangle on screen
    //QPixmap pic;
};

struct KeyLayout
{
    int numKeys;
    KeyInfo *keys;              // pointer to first key info
    QSvgRenderer *svg;
    QRectF rectSvg;
    int scrWidth;               // width and height for which are keys
    int scrHeight;              // currrently placed on the screen
    bool shifted;               // is this uppercase?
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
    explicit KeyboardFrame(QWidget * parent = 0, Qt::WFlags f = 0);
    virtual ~ KeyboardFrame();

    void setLayout(int index = 0);
    void cleanHigh();
    void resetState();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent * e);
    void timerEvent(QTimerEvent * e);
    void hideEvent(QHideEvent *);
    QRect geometryHint() const;
    QSize sizeHint() const;
    bool obscures(const QPoint & point);
    void focusInEvent(QFocusEvent *)
    {
        qWarning() << "Warning: keyboard got focus";
    };
    bool filter(int, int, int, bool, bool)
    {
        return false;
    };
    bool filter(const QPoint &, int, int)
    {
        return false;
    };

signals:
    void needsPositionConfirmation();
    void hiding();
    void showing();

public slots:
    void swapPosition();

private slots:
    void repeat();

private:
    QTimer repeatTimer;
    QVibrateAccessory vib;

    KeyInfo *pressedKey;        // currently pressed key or NULL
    KeyInfo *highKey;           // highlighted key, after release we show the pressed key for some more time
    Qt::KeyboardModifiers modifiers;

    int highTid;                // id for time that hides highlighted key after 200ms
    bool positionTop;
    struct timespec pressTime;  // last time key was pressed
    int caps;                   // direction for shift. -1=caps lock active, 1=caps lock not active
    bool ignorePress;           // used to ignore too fast presses    
    int numLayouts;             // number of currently loaded layouts
    int curLayout;              // current layout
    KeyLayout layouts[MAX_LAYOUTS];
};

#endif
