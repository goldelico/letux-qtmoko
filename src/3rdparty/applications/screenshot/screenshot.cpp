/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <qmenu.h>
#include <qtopia/qsoftmenubar.h>
#include <QContent>
#include <QtopiaApplication>

#include "screenshot.h"

Screenshot::Screenshot( QWidget *parent, Qt::WFlags f)
{
    screenshotLabel = new QLabel;
    screenshotLabel->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);
//    screenshotLabel->setMinimumSize(240, 160);

    createOptionsGroupBox();
    createButtonsLayout();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(screenshotLabel);
    mainLayout->addWidget(optionsGroupBox);
#ifndef Q_WS_QWS
    mainLayout->addLayout(buttonsLayout);
#endif
    setLayout(mainLayout);

    shootScreen();
    delaySpinBox->setValue(5);
    hideThisWindowCheckBox->setChecked(true);
    setWindowTitle(tr("Screenshot"));
#ifndef Q_WS_QWS
    resize(300, 200);
#endif
QtopiaApplication::instance()->registerRunningTask("ReallyFuckingHideThisAndNotKillIt");
    
}

void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    if (!screenshotLabel->pixmap()
            || scaledSize != screenshotLabel->pixmap()->size())
        updateScreenshotLabel();
}

void Screenshot::newScreenshot()
{

    if (hideThisWindowCheckBox->isChecked())
        hide();
#ifndef Q_WS_QWS
    newScreenshotButton->setDisabled(true);
#endif
    QTimer::singleShot(delaySpinBox->value() * 1000, this, SLOT(shootScreen()));
}


void Screenshot::shootScreen()
{
    if (delaySpinBox->value() != 0)
        qApp->beep();
    originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    updateScreenshotLabel();

#ifndef Q_WS_QWS
    newScreenshotButton->setDisabled(false);
#endif
    if (hideThisWindowCheckBox->isChecked())
        show();
}

void Screenshot::saveScreenshot()
{
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;
#ifdef Q_WS_QWS

    QString  fileName = "sc_" + QDateTime::currentDateTime().toString();
    fileName.replace(QRegExp("'"), "");
    fileName.replace(QRegExp(" "), "_");
    fileName.replace(QRegExp(":"), ".");
    fileName.replace(QRegExp(","), "");
    QString dirName = QDir::homePath() + "/Documents/image/png/";

    if ( !QDir( dirName).exists() ) {
        QDir homeDir(QDir::homePath());
        homeDir.mkpath("Documents/image/png");
    }
#else
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(format.toUpper())
                               .arg(format));
    if (!fileName.isEmpty())
#endif    
#ifdef Q_WS_QWS
    QString new_file = dirName+"/"+fileName+".png";
    originalPixmap.save(new_file, format.toAscii());
    QContent content;
    content.setName( fileName);
    content.setFile( new_file );
    content.commit();

#else
        originalPixmap.save(fileName, format.toAscii());
#endif
            

}
void Screenshot::updateCheckBox()
{
    if (delaySpinBox->value() == 0)
        hideThisWindowCheckBox->setDisabled(true);
    else
        hideThisWindowCheckBox->setDisabled(false);
}

void Screenshot::createOptionsGroupBox()
{
    optionsGroupBox = new QGroupBox(tr("Options"));

    delaySpinBox = new QSpinBox;
    delaySpinBox->setSuffix(tr(" s"));
    delaySpinBox->setMaximum(60);
    connect(delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCheckBox()));

    delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

    hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));

    optionsGroupBoxLayout = new QGridLayout;
    optionsGroupBoxLayout->addWidget(delaySpinBoxLabel, 0, 0);
    optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
    optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
    optionsGroupBox->setLayout(optionsGroupBoxLayout);
}

void Screenshot::createButtonsLayout()
{
#ifdef Q_WS_QWS
   QMenu *contextMenu = QSoftMenuBar::menuFor(this);

    contextMenu->addAction( tr("New"), this, SLOT( newScreenshot()));
    contextMenu->addAction( tr("Save"), this, SLOT( saveScreenshot()));
    contextMenu->addAction( tr("Quit"), this, SLOT( close()));
#else
    newScreenshotButton = createButton(tr("New Screenshot"),
                                       this, SLOT(newScreenshot()));

    saveScreenshotButton = createButton(tr("Save Screenshot"),
                                        this, SLOT(saveScreenshot()));

    quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(newScreenshotButton);
    buttonsLayout->addWidget(saveScreenshotButton);
    buttonsLayout->addWidget(quitScreenshotButton);
#endif
                                                       
}

#ifndef Q_WS_QWS
QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->connect(button, SIGNAL(clicked()), receiver, member);
    return button;
}
#endif

void Screenshot::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}
