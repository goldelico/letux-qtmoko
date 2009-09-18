#include "DesktopScanner.h"
#include <QDebug>

DesktopScanner::DesktopScanner()
{
    QString desktopPath = "/usr/share/applications";
    QDir desktopDir = QDir(desktopPath);
    QStringList files;
    files = desktopDir.entryList(QStringList("*.desktop"), QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for (int i=0; i<files.count(); i++)
    {
        ParseFile(desktopPath, files[i]);
    }
}

void DesktopScanner::ParseFile(QString path, QString fname)
{
    QString s = path+"/"+fname;
    QFile f(s);
    if (f.open(QIODevice::ReadOnly))
    {
        DesktopEntry entry;
        QString text = f.readAll();
        lines = text.split("\n");

        entry.name = GetField("Name");
        entry.comment = GetField("Comment");
        entry.icon = FindIcon(GetField("Icon"));
        entry.exec = GetField("Exec");

        entry.file = fname;
        entry.file.truncate(entry.file.length() - 8); // (".desktop").length() == 8

        entries << entry;
    }
}

QString DesktopScanner::GetField(QString s)
{
    for (int i=0; i<lines.count(); i++)
    {
        QString line = lines[i];
        if ( (line.indexOf(s) == 0) &&
             (line.contains("=")) )
        {
            QString value = (line.split("=")[1]).trimmed();
            return value;
        }
    }
    return "";
}

QString DesktopScanner::FindIcon(QString s)
{
    if (s!="")
    {
        QString pixmapsPath = "/usr/share/pixmaps";
        QString icon = pixmapsPath+"/"+s;
        if (QFile::exists(icon+".png")) return icon+".png";
        if (QFile::exists(icon+".svg")) return icon+".svg";
        if (QFile::exists(icon+".xpm")) return icon+".xpm";
        if (QFile::exists(icon)) return icon;
    }
    return "";
}
