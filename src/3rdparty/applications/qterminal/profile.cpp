#include <QtCore/QSettings>
#include "profile.h"

Profile::Profile() {

}

Profile::Profile(const QString &name)
{
  loadProfile(name);
}

Profile::Profile( const QString& name,
                  const QByteArray& iolayerName,
                  const QByteArray& termName,
                  const QString& colorScheme,
                  const QString& keyboard,
                  int terminal )
    : m_name( name ), m_ioLayer( iolayerName ), m_term( termName), m_autoConnect(0),
      m_colorScheme( colorScheme ), m_keyboard( keyboard ), m_terminal( terminal )
{}

void Profile::loadProfile(const QString& name)
{
  QSettings profiles("Trolltech","qterminal");
  
  m_name = name;
  profiles.beginGroup(name);
  m_command = profiles.value("command","/bin/bash").toString();
  m_ioLayer = profiles.value("ioLayer","Pty").toByteArray();
  m_term = profiles.value("term","linux").toByteArray();
  m_autoConnect = profiles.value("autoConnect",0).toInt();
  m_colorScheme = profiles.value("colorScheme","Linux").toString();
  m_keyboard = profiles.value("keyboard", "linux").toString();
  m_terminal = 0;
#ifdef QTOPIA_PHONE
  m_fontName = profiles.value("fontName", "dejavu-mono").toString();
  /* probably too small for Universal (480x640) but good for 240x320...*/
  m_fontSize = profiles.value("fontSize", 5).toInt(); 
#else
  m_fontName = profiles.value("fontName", "Courier New").toString();
  m_fontSize = profiles.value("fontSize", 12).toInt();
#endif
  m_fontStyle = profiles.value("fontStyle", 0).toInt();
  m_cursorShape = profiles.value("cursorShape", 0).toInt();
  m_historyLines = profiles.value("historyLines", 50).toInt();
}

void Profile::saveProfile(const QString& name)
{
  QSettings profiles("Trolltech","qterminal");

  profiles.beginGroup(name);
  profiles.setValue("command",m_command);
  profiles.setValue("ioLayer",QString(m_ioLayer));
  profiles.setValue("term",QString(m_term));
  profiles.setValue("autoConnect",m_autoConnect);
  profiles.setValue("colorScheme",m_colorScheme);
  profiles.setValue("keyboard", m_keyboard);
  profiles.setValue("fontName", m_fontName);
  profiles.setValue("fontSize", m_fontSize);
  profiles.setValue("fontStyle", m_fontStyle);
  profiles.setValue("cursorShape", m_cursorShape);
  profiles.setValue("historyLines", m_historyLines);
  profiles.sync(); 

}

Profile::Profile( const Profile& prof )
{
    (*this) = prof;
}
bool Profile::operator==( const Profile& prof ) {
    if ( m_name == prof.m_name ) return true;

    return false;
}
Profile &Profile::operator=( const Profile& prof ) {
    m_name = prof.m_name;
    m_ioLayer = prof.m_ioLayer;
    m_autoConnect = prof.m_autoConnect;
    m_colorScheme = prof.m_colorScheme;
    m_keyboard = prof.m_keyboard;
    m_terminal = prof.m_terminal;
    m_conf = prof.m_conf;
    m_term = prof.m_term;

    return *this;
}
Profile::~Profile() {
}
QMap<QString, QString> Profile::conf()const {
    return m_conf;
}
QString Profile::name()const {
    return m_name;
}
QString Profile::command()const {
    return m_command;
}
QByteArray Profile::ioLayerName()const {
    return m_ioLayer;
}
QByteArray Profile::terminalName( )const {
    return m_term;
}
bool Profile::autoConnect()const {

    return m_autoConnect;
}
QString Profile::colorScheme()const {
    return m_colorScheme;
}
QString Profile::keyboard()const {
    return m_keyboard;
}
int Profile::terminal()const {
    return m_terminal;
}

QString Profile::fontName()const {
  return m_fontName;
}

int Profile::fontSize()const {
  return m_fontSize;
}

int Profile::fontStyle()const {
  return m_fontStyle;
}

int Profile::cursorShape()const {
  return m_cursorShape;
}

int Profile::historyLines()const {
  return m_historyLines;
}

void Profile::setName( const QString& str ) {
    m_name = str;
}
void Profile::setCommand( const QString& str ) {
    m_command = str;
}
void Profile::setIOLayer( const QByteArray& name ) {
    m_ioLayer = name;
}
void Profile::setTerminalName( const QByteArray& str ) {
    m_term = str;
}
void Profile::setAutoConnect( const bool c) {

    m_autoConnect = c;
}
void Profile::setColorScheme( QString& colorScheme ) {
    m_colorScheme = colorScheme;
}
void Profile::setKeyboard( QString& keyboard ) {
    m_keyboard = keyboard;
}
void Profile::setTerminal( int term ) {
    m_terminal =  term;
}

void Profile::setFontName( QString& fontName ) {
  m_fontName = fontName;
}

void Profile::setFontSize( int size ) {
  m_fontSize = size;
}

void Profile::setFontStyle( int style ) {
  m_fontStyle = style;
}

void Profile::setCursorShape( int shape ) {
  m_cursorShape = shape;
}

void Profile::setHistoryLines( int lines ) {
  m_historyLines = lines;
}

/* config stuff */
void Profile::clearConf() {
    m_conf.clear();
}
void Profile::writeEntry( const QString& key,  const QString& value ) {
    m_conf.remove(key);
    m_conf.insert(key, value);
    //m_conf.replace( key, value );
}
void Profile::writeEntry( const QString& key, int num ) {
    writeEntry( key,  QString::number( num ) );
}
void Profile::writeEntry( const QString& key, bool b ) {
    writeEntry( key, QString::number(b) );
}
void Profile::writeEntry( const QString& key, const QStringList& lis, const QChar& sep ) {
    writeEntry( key, lis.join(sep) );
}
QString Profile::readEntry( const QString& key,  const QString& deflt )const {
    QMap<QString, QString>::ConstIterator it;
    it = m_conf.find( key );

    if ( it != m_conf.end() )
        return it.value();

    return deflt;
}
int Profile::readNumEntry( const QString& key, int def )const {
    QMap<QString, QString>::ConstIterator it;
    it = m_conf.find( key );

    if ( it != m_conf.end() ) {
        bool ok;
        int val = it.value().toInt(&ok);

        if (ok)
            return val;
    }
    return def;
}
bool Profile::readBoolEntry( const QString& key,  bool def )const {
    return readNumEntry( key, def );
}
void Profile::setConf( const QMap<QString, QString>& conf ) {
    m_conf = conf;
};
