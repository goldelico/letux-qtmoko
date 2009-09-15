#include "apprunningscreen.h"

AppRunningScreen::AppRunningScreen()
{
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
        p.drawText(this->rect(), Qt::AlignCenter, tr("press AUX to leave"));
    }
}

void AppRunningScreen::enterFullScreen()
{
#ifdef QTOPIA
    // Show editor view in full screen
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    raise();
#endif
}
