#ifndef DESKTOP_SCANNER_H
#define DESKTOP_SCANNER_H

#include <QDir>
#include <QList>
#include <QStringList>

struct DesktopEntry
{
    QString name, comment, icon, exec, file;
};

class DesktopScanner
{
    public:
        DesktopScanner();
        QList<DesktopEntry> entries;
    private:
        void ParseFile(QString path, QString fname);
        QString GetField(QString s);
        QString FindIcon(QString s);
        QStringList lines;
};

#endif //DESKTOP_SCANNER_H
