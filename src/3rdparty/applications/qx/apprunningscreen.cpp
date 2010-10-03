#include "apprunningscreen.h"

AppRunningScreen::AppRunningScreen()
{
}

void AppRunningScreen::showScreen(bool fullscreen, bool kbd)
{
#ifdef QTOPIA
    QtopiaApplication::setInputMethodHint
            (this, kbd ?
             QtopiaApplication::AlwaysOn :
             QtopiaApplication::AlwaysOff);
#else
    Q_UNUSED(kbd);
#endif
    showMaximized();
    if(fullscreen)
    {
        enterFullScreen();
    }
}

bool AppRunningScreen::event(QEvent *event)
{
#ifdef QTOPIA
    if(event->type() == QEvent::WindowDeactivate)
    {
        pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
        lower();
        emit deactivated();
    }
    else if(event->type() == QEvent::WindowActivate)
    {
        QString title = windowTitle();
        setWindowTitle(QLatin1String("_allow_on_top_"));
        raise();
        setWindowTitle(title);
    }
#endif
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

void AppRunningScreen::keyPressEvent(QKeyEvent *e)
{
    emit keyPress(e);
}

void AppRunningScreen::keyReleaseEvent(QKeyEvent *e)
{
    emit keyRelease(e);
}

void AppRunningScreen::resizeEvent(QResizeEvent *)
{
    //qDebug() << e->size().width() << "x" << e->size().height();
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
