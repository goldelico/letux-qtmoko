#include "loginwindow.h"
#include <QApplication>
#include <QSoftMenuBar>

#include <QVBoxLayout>
#include <QGridLayout>

#include "googlesync.h"
#include "googlesession.h"

LoginWindow::LoginWindow(QWidget *parent, Qt::WindowFlags wf) 
  : QMainWindow(parent, wf)
{
    QSoftMenuBar::menuFor( this );

    cfg = new QSettings ("ezxdev.org","google");

    QGridLayout *grid = new QGridLayout;

    state      = new QLabel(this);
    login = new QLineEdit(cfg->value("login/email", tr("Email")).toString(),this );
    passw = new QLineEdit(cfg->value("login/password", tr("Password")).toString(),this);

    startButton =   new QPushButton("Start sync",this);
    exitButton  =   new QPushButton("Exit",this);

    save = new QCheckBox("Save password", this);
    save->setCheckState( (Qt::CheckState) cfg->value("login/save" ).toInt() );

    skip = new QCheckBox("Skip without numbers", this);
    skip->setCheckState( (Qt::CheckState) cfg->value("login/skip" ).toInt() );

    connect( startButton, SIGNAL( clicked() ),
             SLOT( start () ) );

    connect( exitButton,  SIGNAL( clicked() ),
             qApp, SLOT( quit () ) );

    grid->addWidget(login,0,0);
    grid->addWidget(passw,1,0);

    grid->addWidget(state, 2, 0);
    grid->addWidget(skip, 3, 0);
    grid->addWidget(save, 4, 0);
    grid->addWidget(startButton, 5, 0);
    grid->addWidget(exitButton, 6, 0);
    grid->setSizeConstraint(QLayout::SetMaximumSize);
    QWidget *central = new QWidget();
    central->setLayout(grid);

    setCentralWidget(central);

    sync = new GoogleSync();

}


void LoginWindow::start()
{
      // Qt::Checked  
    cfg->setValue("login/save", (int) save->checkState() );
    cfg->setValue("login/skip", (int) skip->checkState() );
    cfg->setValue("login/email", login->text() );
    if (save->checkState() == Qt::Checked )
        cfg->setValue("login/password", passw->text() );

    connect(sync, SIGNAL(stateChanged(GoogleSession::State) ),
            this, SLOT(stateChanged(GoogleSession::State) ) );

    sync->start( 
        login->text() , 
        passw->text() ,
        (bool) skip->checkState()
        );
}


void LoginWindow::stateChanged(GoogleSession::State s)
{
    state->setText(GoogleSession::stateName(s) );
}
