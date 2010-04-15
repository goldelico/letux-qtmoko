#include "neocontrol.h"

NeoControl::NeoControl(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QTOPIA
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
    bQvga = new QPushButton(tr("Switch to QVGA"), this);
    connect(bQvga, SIGNAL(clicked()), this, SLOT(qvgaClicked()));

    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bBack);

    layout = new QVBoxLayout(this);
    layout->addWidget(bQvga);
    layout->addLayout(buttonLayout);

    showScreen(NeoControl::ScreenInit);
}

NeoControl::~NeoControl()
{

}

void NeoControl::backClicked()
{
    if(screen == NeoControl::ScreenInit)
    {
        close();
    }
}

void NeoControl::qvgaClicked()
{
    QFile cal("/etc/pointercal_qvga");
    if(!cal.exists())
    {
        cal.open(QFile::WriteOnly);
        cal.write("-360 16920 -2227050 -21726 -170 20270704 56321");
        cal.close();
    }

    QFile f("/tmp/restart-qtopia-qvga");
    f.open(QFile::WriteOnly);
    f.write("restart in qvga");
    f.close();
    QMessageBox::information(this, tr("QVGA setup"), tr("QVGA mode will be activated after restarting QtExtended with POWER button"));
}

void NeoControl::showScreen(NeoControl::Screen scr)
{
    this->screen = scr;

    bQvga->setVisible(scr == NeoControl::ScreenInit);
    bBack->setVisible(scr == NeoControl::ScreenInit || scr == NeoControl::ScreenDisplay);
    lineEdit->setVisible(false);

    switch(scr)
    {
        case NeoControl::ScreenInit:
            bBack->setText(tr("Quit"));
            break;
        case NeoControl::ScreenDisplay:
            bBack->setText(tr("Back"));
            break;
    }
}
