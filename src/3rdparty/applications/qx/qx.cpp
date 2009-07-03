#include "qx.h"

QX::QX(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QT_QWS_FICGTA01
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
    bOk = new QPushButton(this);
    bOk->setText("Start");
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    label = new QLabel(this);
    label->setText("Command");

    lineEdit = new QLineEdit(this);
    lineEdit->setText("xclock");

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bOk);
    buttonLayout->addWidget(bBack);

    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addLayout(buttonLayout);

    fullScreen = false;
}

QX::~QX()
{

}

void QX::resizeEvent(QResizeEvent *)
{
    if(size() != qApp->desktop()->size()) {
        // Widget is not the correct size, so do the fullscreen magic
         enableFullscreen();
    }
}

void QX::focusInEvent(QFocusEvent *) {
    // Always do it here, no matter the size.
    enableFullscreen();
}

void QX::enableFullscreen() {
    if(!fullScreen)
    {
        return;
    }
    // Make sure size is correct
    setFixedSize(qApp->desktop()->size());
    // This call is needed because showFullScreen won't work
    // correctly if the widget already considers itself to be fullscreen.
    showNormal();
    // This is needed because showNormal() forcefully changes the window
    // style to WSTyle_TopLevel.
    //reparent(0, WStyle_Customize | WStyle_NoBorder, QPoint(0,0));
    // Enable fullscreen.
    showFullScreen();
}


void QX::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void QX::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    exitFullScreen();
    process = NULL;
}

void QX::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Back:
        {
            close();
            if(process != NULL)
            {
                 process->close();
                 process = NULL;
            }
            e->accept();
        }
        break;
    }
}

void QX::okClicked()
{
//    resize(qApp->desktop()->size());
//    fullScreen = true;
//    enableFullscreen();
    enterFullScreen();

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(lineEdit->text(), NULL);
}

void QX::backClicked()
{
}

bool QX::event(QEvent *event)
{
    if( event->type() == QEvent::WindowDeactivate && is_fullscreen )
    {
        lower();
    }
    else if( event->type() == QEvent::WindowActivate && is_fullscreen )
    {
        QString title = windowTitle();

        setWindowTitle( QLatin1String( "_allow_on_top_" ) );

        raise();

        setWindowTitle( title );
    }

     return QWidget::event( event );
}

void QX::enterFullScreen()
{
    // Show editor view in full screen
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);

    raise();

    //emit fullScreenDisabled(false);

    is_fullscreen = true;
}

void QX::exitFullScreen()
{
    is_fullscreen = false;

    //emit showFullScreenWidgets(m_fullScreenWidgetsVisible = false);
    //emit fullScreenDisabled(true);

    //QTimer::singleShot(0, this, SLOT(showMaximized()));
}


