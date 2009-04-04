#ifndef OPIE_PROFILE_H
#define OPIE_PROFILE_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
/**
 * A session will be generated from a saved
 * profile. A profile contains the iolayername
 * a name.
 * We can generate a Session from a Profile
 * Configuration is contained here too
 */
class Profile {
public:
    typedef QList<Profile> ValueList;
    enum Color { Black = 0,
                 White,
                 Gray,
                 Green,
                 Orange};
    enum Terminal {VT102 = 0, VT100,
                   Ansi,
                   Linux,
                   XTerm };
    enum Font { Micro = 0,  Small, Medium };
    Profile();
    Profile( const QString& name,
             const QByteArray& iolayerName,
             const QByteArray& termName,
             const QString& colorScheme,
             const QString& keyboard,
             int terminal);
    Profile( const QString &name);
    Profile( const Profile& );
    Profile &operator=( const Profile& );
    bool operator==( const Profile& prof );
     
    ~Profile();
    void loadProfile(const QString& name);
    void saveProfile(const QString& name);
    QString name()const;
    QString command()const;
    QByteArray ioLayerName()const;
    QByteArray terminalName()const;
    bool autoConnect()const;
    QString colorScheme()const;
    QString keyboard()const;
    QString fontName()const;
    int fontSize()const;
    int fontStyle()const;
    int cursorShape()const;
    int historyLines()const;
    int terminal()const;

    /*
     * config stuff
     */
    QMap<QString, QString> conf()const;
    void clearConf();
    void writeEntry( const QString& key,  const QString& value );
    void writeEntry( const QString& key,  int num );
    void writeEntry( const QString& key, bool b );
    void writeEntry( const QString& key, const QStringList&, const QChar& );
    QString readEntry( const QString& key, const QString& deflt = QString::null)const;
    int readNumEntry( const QString& key, int = -1 )const;
    bool readBoolEntry( const QString& key,  bool = FALSE )const;

    void setName( const QString& );
    void setCommand( const QString& );
    void setIOLayer( const QByteArray& );
    void setTerminalName( const QByteArray& );
    void setAutoConnect( const bool );
    void setColorScheme( QString& colors );
    void setKeyboard( QString& keyb );
    void setFontName( QString& fontName );
    void setFontSize( int size );
    void setFontStyle( int style );
    void setCursorShape( int shape );
    void setHistoryLines( int lines );
    void setTerminal( int term );
    void setConf( const QMap<QString, QString>& );
private:
    QMap<QString, QString> m_conf;
    QString m_name;
    QString m_command;
    QByteArray m_ioLayer, m_term;
    bool m_autoConnect;
    QString m_colorScheme;
    QString m_keyboard;
    QString m_fontName;
    int m_fontSize;
    int m_fontStyle; 
    int m_cursorShape;
    int m_historyLines;
    int m_terminal;
};

#endif
