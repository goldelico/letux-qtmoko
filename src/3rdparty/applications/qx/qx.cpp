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
    bOk->setText("Full screen");
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);

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

void QX::okClicked()
{
    resize(qApp->desktop()->size());
    fullScreen = true;
    enableFullscreen();
}

void QX::backClicked()
{
}
