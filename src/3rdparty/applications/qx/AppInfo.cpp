#include "AppInfo.h"
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDebug>

InfoDialog::InfoDialog(DesktopEntry entry, QWidget *parent) : QDialog(parent)
{
    setFixedSize(470,240);
    setModal(true);
    setWindowTitle(tr("App Info"));

    QString s =
            "<HTML><BODY>"
            "<table width=\"100%\" cellpadding=\"1\" cellspacing=\"0\" border=\"1\">"
            "<tr>"
            "  <td><i>Entry</i></td>"
            "  <td>" + entry.file + ".desktop" + "</td>"
            "</tr>"
            "<tr>"
            "  <td><i>Name</i></td>"
            "  <td>" + entry.name + "</td>"
            "</tr>"
            "<tr>"
            "  <td><i>Comment</i></td>"
            "  <td>" + entry.comment + "</td>"
            "</tr>"
            "<tr>"
            "  <td><i>Exec</i></td>"
            "  <td>" + entry.exec + "</td>"
            "</tr>"
            "</table>"
            "</BODY></HTML>";

    QTextBrowser *txt = new QTextBrowser(this);
    txt->setText(s);

    QVBoxLayout *vb = new QVBoxLayout(this);
    vb->addWidget(txt);
}
