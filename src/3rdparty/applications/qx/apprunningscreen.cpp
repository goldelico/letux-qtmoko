#include "apprunningscreen.h"

AppRunningScreen::AppRunningScreen()
{
    killTimer = new QTimer(this);
    killTimer->setSingleShot(true);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(killTimerElapsed()));
}

void AppRunningScreen::showScreen()
{
    showMaximized();
    enterFullScreen();
}

bool AppRunningScreen::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
    {
        lower();
    }
    else if(event->type() == QEvent::WindowActivate)
    {
        QString title = windowTitle();
        setWindowTitle(QLatin1String("_allow_on_top_"));
        raise();
        setWindowTitle(title);
    }
    return QWidget::event(event);
}

void AppRunningScreen::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if(pixmap.width() > 0)
    {
        p.drawPixmap(0, 0, pixmap);
    }
    else
    {
        p.drawText(this->rect(), Qt::AlignCenter, tr("press screen for 5 secs. to leave"));
    }
}

void AppRunningScreen::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if(killTimer->isActive())
    {
        killTimer->stop();
    }
    killTimer->start(4000);
}

void AppRunningScreen::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    killTimer->stop();
}

void AppRunningScreen::killTimerElapsed()
{
    emit longPress();
}

void AppRunningScreen::enterFullScreen()
{
#ifdef QT_QWS_FICGTA01
    // Show editor view in full screen
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    raise();
#endif
}
