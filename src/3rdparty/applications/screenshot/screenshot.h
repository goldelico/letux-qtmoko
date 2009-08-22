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

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QPixmap>
#include <QWidget>
#include <QHttp>

class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot( QWidget *parent = 0, Qt::WFlags f = 0);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void newScreenshot();
    void saveScreenshot();
    void uploadScreenshot();
    void shootScreen();
    void updateCheckBox();
    void showUploadResult(bool err);

private:
    void createOptionsGroupBox();
    void createButtonsLayout();
#ifndef Q_WS_QWS
    QPushButton *createButton(const QString &text, QWidget *receiver,
                              const char *member);
#endif    
    void updateScreenshotLabel();

    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QGroupBox *optionsGroupBox;
    QSpinBox *delaySpinBox;
    QLabel *delaySpinBoxLabel;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
    QPushButton *saveScreenshotButton;
    QPushButton *uploadScreenshotButton;
    QPushButton *quitScreenshotButton;

    QVBoxLayout *mainLayout;
    QGridLayout *optionsGroupBoxLayout;

    QHttp *http;
    QAction *uploadAction;

#ifndef Q_WS_QWS
    QHBoxLayout *buttonsLayout;
#endif    
};

#endif
