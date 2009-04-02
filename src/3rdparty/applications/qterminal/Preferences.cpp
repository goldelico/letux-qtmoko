
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QFontDatabase>
#ifndef QTOPIA_PHONE
#include <QtGui/QPushButton>
#endif
#include <QDebug>
#include "KeyboardTranslator.h"
#include "ColorScheme.h"
#include "Preferences.h"

Preferences::Preferences(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
  currentProfile = 0;
}

Preferences::Preferences(Profile *p)
  : QDialog(0,0)
{
  currentProfile = p;
  setupUI();
}

Preferences::~Preferences()
{
}

void Preferences::setCurrentProfile(Profile *p)
{
  currentProfile = p;
  setupUI();
}

void Preferences::setupUI()
{
  QWidget *mainWidget = new QWidget();
  QLineEdit *cmd = new QLineEdit();
  QSpinBox *hlines = new QSpinBox(); 
  QComboBox *cursor = new QComboBox();
  QComboBox *fontName = new QComboBox();
  QSpinBox *fontSize = new QSpinBox(); 
  QComboBox *colors = new QComboBox();
  QComboBox *keyboard = new QComboBox();
  QGroupBox *prev = new QGroupBox();
  preview = new QLabel();
  preview->setText("This is a preview");
  preview->setAutoFillBackground(true);
  QVBoxLayout *pl = new QVBoxLayout(prev);
  pl->addWidget(preview);
  QGridLayout *gl = new QGridLayout(mainWidget);
  QVBoxLayout *vl = new QVBoxLayout();
 
  cursor->addItem("Block"); 
  cursor->addItem("Underline"); 
  cursor->addItem("IBeam"); 
  QFontDatabase db;
  fontName->addItems(db.families()); 
  keyboard->addItems(KeyboardTranslatorManager::instance()->allTranslators());
  colors->addItems(ColorSchemeManager::instance()->allColorSchemes());
  hlines->setMinimum(-1);
  hlines->setMaximum(999999);
  
  gl->addWidget(new QLabel("Command"),0,0,1,1);
  gl->addWidget(cmd,0,1,1,1);
  gl->addWidget(new QLabel("History lines:"), 1,0,1,1);
  gl->addWidget(hlines, 1,1,1,1);
  gl->addWidget(new QLabel("Cursor shape:"), 2,0,1,1);
  gl->addWidget(cursor, 2,1,1,1);
  gl->addWidget(new QLabel("Font name:"), 3,0,1,1);
  gl->addWidget(fontName, 3,1,1,1);
  gl->addWidget(new QLabel("Font size:"), 4,0,1,1);
  gl->addWidget(fontSize, 4,1,1,1);
  gl->addWidget(new QLabel("Color scheme:"), 5,0,1,1);
  gl->addWidget(colors, 5,1,1,1);
  gl->addWidget(new QLabel("Keyboard:"), 6,0,1,1);
  gl->addWidget(keyboard, 6,1,1,1);
  gl->addWidget(prev, 7,0,1,2); 
  vl->addWidget(mainWidget);

#ifndef QTOPIA_PHONE
  QPushButton *bOk = new QPushButton("Ok");
  QPushButton *bCancel = new QPushButton("Cancel");
  QHBoxLayout *hl = new QHBoxLayout();
  hl->addWidget(bOk);
  hl->addWidget(bCancel);
  connect(bOk, SIGNAL(clicked()), this, SLOT(accept()));
  connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));
  vl->addLayout(hl);
#endif

  if (currentProfile != 0) {
    cmd->setText(currentProfile->command());
    hlines->setValue(currentProfile->historyLines());
    cursor->setCurrentIndex(currentProfile->cursorShape());
    int x = fontName->findText(currentProfile->fontName());
    if (x>=0)
      fontName->setCurrentIndex(x);
    fontSize->setValue(currentProfile->fontSize());
    x = keyboard->findText(currentProfile->keyboard());
    if (x>=0)
      keyboard->setCurrentIndex(x);
    x = colors->findText(currentProfile->colorScheme());
    if (x>=0)
      colors->setCurrentIndex(x);
    updatePreview();

    connect(cmd, SIGNAL(textChanged(const QString&)), this, SLOT(setCommand(const QString&)));
    connect(cursor, SIGNAL(activated(int)), this, SLOT(setCursorShape(int)));
    connect(fontSize, SIGNAL(valueChanged(int)), this, SLOT(setFontSize(int)));
    connect(fontName, SIGNAL(activated(QString)), this, SLOT(setFontName(QString)));
    connect(colors, SIGNAL(activated(QString)), this, SLOT(setColorScheme(QString)));
    connect(hlines, SIGNAL(valueChanged(int)), this, SLOT(setHistoryLines(int)));
  }
  setWindowTitle("Current profile");  
  setLayout(vl);
#ifndef QTOPIA_PHONE
  adjustSize();
#endif
}

void Preferences::updatePreview()
{
  if (currentProfile == 0) return;
  QFont f(currentProfile->fontName(), currentProfile->fontSize());
  const ColorScheme *cs = ColorSchemeManager::instance()->findColorScheme(currentProfile->colorScheme());
  ColorEntry ce = cs->colorEntry(0);
  QPalette p(preview->palette());
  p.setColor(QPalette::WindowText, ce.color);
  f.setBold(ce.bold);
  ce = cs->colorEntry(1);
  p.setColor(QPalette::Background, ce.color);
  preview->setPalette(p);
  preview->setFont(f);
}

void Preferences::setFontName(QString name)
{
  if (currentProfile == 0) return;
  currentProfile->setFontName(name);
  updatePreview();
}

void Preferences::setFontSize(int value)
{
  if (currentProfile == 0) return;
  currentProfile->setFontSize(value);
  updatePreview();
}

void Preferences::setCursorShape(int value)
{
  if (currentProfile == 0) return;
  currentProfile->setCursorShape(value);
  updatePreview();
}

void Preferences::setColorScheme(QString name)
{
  if (currentProfile == 0) return;
  currentProfile->setColorScheme(name);
  updatePreview();
}

void Preferences::setHistoryLines(int value)
{
  currentProfile->setHistoryLines(value);
}

void Preferences::setCommand(const QString& value)
{
  currentProfile->setCommand(value);
}

Profile *Preferences::profile()
{
  return currentProfile;
}
