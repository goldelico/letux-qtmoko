#include <qstring.h>
#include <qlayout.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qfont.h>
#include <qsettings.h>

#include <assert.h>

#include "krfbconnection.h"
#include "kvncoptionsdlg.h"
#include "kvncconnectdlg.h"

#include <QDebug>


KVNCConnectDlg::KVNCConnectDlg( KRFBConnection *con,
				QWidget *parent )
  : QDialog( parent )
{
    assert( con );
    this->con = con;

    setWindowTitle( tr("VNC Viewer") );
    setVisible(true);
    setFocusPolicy( Qt::StrongFocus );
    setFocus();
    showFullScreen();
    QGridLayout *inner = new QGridLayout( this );
    inner->setSpacing(6);
    QLabel *label = new QLabel( tr("Host:"), this , 0);
    hostNameCombo = new QComboBox( this );
    hostNameCombo->setInsertPolicy( QComboBox::InsertAtTop );
    hostNameCombo->setEditable(true);
    hostNameCombo->setMaxCount( 5 );
    hostNameCombo->setFocus();

    QSettings config("Trolltech","keypebble");
    config.beginGroup( "Connection" );
    QStringList hlist = config.value( "Hosts").toString().split( ',' );

    if ( hlist.count() < 2 ) {
	hostNameCombo->addItem( "localhost" );
	hostNameCombo->addItem( "host1" );
	hostNameCombo->addItem( "host2" );
	hostNameCombo->addItem( "host3" );
	hostNameCombo->addItem( "host4" );
    } else {
	hostNameCombo->addItems( hlist );
    }

    inner->addWidget( label, 0, 0 );
    inner->addWidget( hostNameCombo, 0, 1 );

    label = new QLabel( tr("Display:"), this, 0 );
    displayNumberEdit = new QSpinBox( this );

    inner->addWidget( label, 1, 0 );
    inner->addWidget( displayNumberEdit, 1, 1 );

    label = new QLabel( tr("Password:"), this );
    inner->addWidget( label, 2, 0 );

    passwordEdit = new QLineEdit( this );
    passwordEdit->setEchoMode( QLineEdit::Password );
    inner->addWidget( passwordEdit, 2, 1 );

    inner->setColumnStretch( 0, 0 );
    inner->setColumnStretch( 1, 15 );
}


void KVNCConnectDlg::accept()
{
    int dis;
    QString temp = displayNumberEdit->text();
    if(temp.isEmpty())
	dis = -1;
    else
	dis = temp.toUInt();    

    QSettings config("Trolltech","keypebble");
    config.beginGroup( "Connection" );
    QStringList hlist;
    hlist.append(hostname());
    for (int i=0; i<hostNameCombo->count(); i++) {
        if(hlist.indexOf(hostNameCombo->itemText(i)) == -1)
          hlist.append(hostNameCombo->itemText(i));
    }
    config.setValue("Hosts", hlist.join(QString(',' )));
    config.sync();
    QDialog::accept();
}

void KVNCConnectDlg::options()
{
  KVNCOptionsDlg *wdg = new KVNCOptionsDlg( con->options() );
  wdg->exec();
  delete wdg;
}

void KVNCConnectDlg::hideEvent(QHideEvent *e )
{
  emit accepted();
  QDialog::hideEvent(e);
}

