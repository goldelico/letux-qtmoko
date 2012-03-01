#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ofonoutil.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        oManager("org.ofono", "/", QDBusConnection::systemBus(), this),
        oCellBroadcast("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oConnMan("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oMessageManager("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oNetReg("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oPhoneBook("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oPushNotify("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oRadio("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oSim("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oSuplServices("org.ofono", "/hso_0", QDBusConnection::systemBus(), this),
        oCallManager("org.ofono", "/hso_0", QDBusConnection::systemBus(), this)
{
    ui->setupUi(this);

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::checkIface(QDBusAbstractInterface *iface)
{
    QString status = (iface->isValid() ? "OK" : "INVALID");
    ui->tbInit->append(status + " " + iface->service() + " " + iface->path());
}

QString variantMapToStr(QVariantMap map)
{
    QString str;
    for(int i = 0; i < map.count(); i++)
    {
        QString key = map.keys().at(i);
        str += key + ": " + map.value(key).toString() + "\n";
    }
    return str;
}

QString ofonoObjectToStr(QOFonoObject o)
{
    QString str;
    str.append(o.object.path());
    str.append("\n");
    str.append(variantMapToStr(o.properties));
    str.append("\n\n");
    return str;
}

QString ofonoObjectListToStr(QOFonoObjectList list)
{
    QString str;
    for(int i = 0; i < list.count(); i++)
    {
        str.append(ofonoObjectToStr(list.at(i)));
    }
    return str;
}

void MainWindow::showVariantMap(QVariantMap map, QString caption)
{
    QMessageBox::information(this, caption, variantMapToStr(map));
}

void MainWindow::showVariantMapResult(QOFonoDBusPendingReply<QVariantMap> reply, QString caption)
{
    showVariantMap(reply.value(), caption);
}

void MainWindow::on_cbRedLed_stateChanged(int state)
{

}

void MainWindow::on_cbBlueLed_stateChanged(int state)
{

}

void MainWindow::on_cbOrangeLed_stateChanged(int state)
{

}

void MainWindow::refresh()
{
    // Init tab
    if(ui->tabInit->isVisible())
    {
        ui->tbInit->clear();
        checkIface(&oManager);
        checkIface(&oConnMan);
        checkIface(&oMessageManager);
        checkIface(&oNetReg);
        checkIface(&oPhoneBook);
        checkIface(&oPushNotify);
        checkIface(&oRadio);
        checkIface(&oSim);
        checkIface(&oSuplServices);
        checkIface(&oCallManager);
    }
    if(ui->tabManager->isVisible())
    {
        QOFonoDBusPendingReply<QOFonoObjectList> reply = oManager.GetModems();
        checkReply2(reply, false, true);
        QOFonoObjectList modems = reply.value();
        ui->lModems->setText(ofonoObjectListToStr(modems));
    }

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

void MainWindow::on_bRegister_clicked()
{
}

void MainWindow::on_bUnregister_clicked()
{
}

void MainWindow::on_bCall_clicked()
{
}

void MainWindow::on_bGsmFeatures_clicked()
{
}

void MainWindow::on_bListProviders_clicked()
{
}

void MainWindow::gsmCallStatusChange(int id, const QString &status, const QVariantMap &properties)
{
}

void MainWindow::incomingUssd(const QString &mode, const QString &message)
{
}

void MainWindow::on_bUssdReq_clicked()
{
}

void MainWindow::on_bActivateCall_clicked()
{
}

void MainWindow::on_bActivateConference_clicked()
{
}

void MainWindow::on_bRelease_clicked()
{
}

void MainWindow::on_bHoldActive_clicked()
{
}

void MainWindow::on_bJoin_clicked()
{
}

void MainWindow::on_bReleaseHeld_clicked()
{
}

void MainWindow::on_bReleaseAll_clicked()
{
}

void MainWindow::on_bTransfer_clicked()
{
}

void MainWindow::incomingTextMessage(const QString &number, const QString &timestamp, const QString &contents)
{
    QMessageBox::information(this, "Incomming SMS",
                             "From: " + number + "\n" +
                             "Time: " + timestamp + "\n" +
                             contents);
}

void MainWindow::incomingMessageReport(int reference, const QString &status, const QString &sender_number, const QString &contents)
{
    QMessageBox::information(this, "SMS status report",
                             "Reference: " + QString::number(reference) + "\n" +
                             "Status: " + status + "\n" +
                             "Sender number: " + sender_number + "\n" +
                             contents);
}

void MainWindow::on_bSend_clicked()
{
}

void MainWindow::on_bGetFunctionality_clicked()
{
}

void MainWindow::on_bGetStatus_clicked()
{
}

void MainWindow::on_tbSmsContent_textChanged()
{
}

void MainWindow::gsmMessageSizeFinished(QOFonoDBusPendingCall & call)
{
}

void MainWindow::on_bQueryMessages_clicked()
{
}

void MainWindow::on_bSimInfo_clicked()
{
}

void MainWindow::on_bSetCredentials_clicked()
{
}

void MainWindow::on_tbActivateContext_clicked()
{
}

void MainWindow::on_bDeactivateContext_clicked()
{
}

void MainWindow::on_bRetrieveTextMessages_clicked()
{
}
