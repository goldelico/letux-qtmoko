/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Jan 4 14:46:45 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *b1;
    QPushButton *b2;
    QPushButton *b3;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *b4;
    QPushButton *b5;
    QPushButton *b6;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *b7;
    QPushButton *b8;
    QPushButton *b9;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_23;
    QPushButton *b0;
    QPushButton *pushButton_25;
    QProgressBar *progressBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 640);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(64);
        label->setFont(font);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetDefaultConstraint);
        b1 = new QPushButton(centralWidget);
        b1->setObjectName(QString::fromUtf8("b1"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(b1->sizePolicy().hasHeightForWidth());
        b1->setSizePolicy(sizePolicy1);
        b1->setMinimumSize(QSize(0, 0));
        b1->setFont(font);

        horizontalLayout_6->addWidget(b1);

        b2 = new QPushButton(centralWidget);
        b2->setObjectName(QString::fromUtf8("b2"));
        sizePolicy1.setHeightForWidth(b2->sizePolicy().hasHeightForWidth());
        b2->setSizePolicy(sizePolicy1);
        b2->setFont(font);

        horizontalLayout_6->addWidget(b2);

        b3 = new QPushButton(centralWidget);
        b3->setObjectName(QString::fromUtf8("b3"));
        sizePolicy1.setHeightForWidth(b3->sizePolicy().hasHeightForWidth());
        b3->setSizePolicy(sizePolicy1);
        b3->setFont(font);

        horizontalLayout_6->addWidget(b3);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetDefaultConstraint);
        b4 = new QPushButton(centralWidget);
        b4->setObjectName(QString::fromUtf8("b4"));
        sizePolicy1.setHeightForWidth(b4->sizePolicy().hasHeightForWidth());
        b4->setSizePolicy(sizePolicy1);
        b4->setMinimumSize(QSize(0, 0));
        b4->setFont(font);

        horizontalLayout_8->addWidget(b4);

        b5 = new QPushButton(centralWidget);
        b5->setObjectName(QString::fromUtf8("b5"));
        sizePolicy1.setHeightForWidth(b5->sizePolicy().hasHeightForWidth());
        b5->setSizePolicy(sizePolicy1);
        b5->setFont(font);

        horizontalLayout_8->addWidget(b5);

        b6 = new QPushButton(centralWidget);
        b6->setObjectName(QString::fromUtf8("b6"));
        sizePolicy1.setHeightForWidth(b6->sizePolicy().hasHeightForWidth());
        b6->setSizePolicy(sizePolicy1);
        b6->setFont(font);

        horizontalLayout_8->addWidget(b6);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setSizeConstraint(QLayout::SetDefaultConstraint);
        b7 = new QPushButton(centralWidget);
        b7->setObjectName(QString::fromUtf8("b7"));
        sizePolicy1.setHeightForWidth(b7->sizePolicy().hasHeightForWidth());
        b7->setSizePolicy(sizePolicy1);
        b7->setMinimumSize(QSize(0, 0));
        b7->setFont(font);

        horizontalLayout_9->addWidget(b7);

        b8 = new QPushButton(centralWidget);
        b8->setObjectName(QString::fromUtf8("b8"));
        sizePolicy1.setHeightForWidth(b8->sizePolicy().hasHeightForWidth());
        b8->setSizePolicy(sizePolicy1);
        b8->setFont(font);

        horizontalLayout_9->addWidget(b8);

        b9 = new QPushButton(centralWidget);
        b9->setObjectName(QString::fromUtf8("b9"));
        sizePolicy1.setHeightForWidth(b9->sizePolicy().hasHeightForWidth());
        b9->setSizePolicy(sizePolicy1);
        b9->setFont(font);

        horizontalLayout_9->addWidget(b9);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton_23 = new QPushButton(centralWidget);
        pushButton_23->setObjectName(QString::fromUtf8("pushButton_23"));
        sizePolicy1.setHeightForWidth(pushButton_23->sizePolicy().hasHeightForWidth());
        pushButton_23->setSizePolicy(sizePolicy1);
        pushButton_23->setMinimumSize(QSize(0, 0));
        pushButton_23->setFont(font);

        horizontalLayout_10->addWidget(pushButton_23);

        b0 = new QPushButton(centralWidget);
        b0->setObjectName(QString::fromUtf8("b0"));
        sizePolicy1.setHeightForWidth(b0->sizePolicy().hasHeightForWidth());
        b0->setSizePolicy(sizePolicy1);
        b0->setFont(font);

        horizontalLayout_10->addWidget(b0);

        pushButton_25 = new QPushButton(centralWidget);
        pushButton_25->setObjectName(QString::fromUtf8("pushButton_25"));
        sizePolicy1.setHeightForWidth(pushButton_25->sizePolicy().hasHeightForWidth());
        pushButton_25->setSizePolicy(sizePolicy1);
        pushButton_25->setFont(font);

        horizontalLayout_10->addWidget(pushButton_25);


        verticalLayout->addLayout(horizontalLayout_10);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximum(10);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        MainWindow->setCentralWidget(centralWidget);
        progressBar->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "7+3=", 0, QApplication::UnicodeUTF8));
        b1->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        b2->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
        b3->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
        b4->setText(QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8));
        b5->setText(QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8));
        b6->setText(QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8));
        b7->setText(QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8));
        b8->setText(QApplication::translate("MainWindow", "8", 0, QApplication::UnicodeUTF8));
        b9->setText(QApplication::translate("MainWindow", "9", 0, QApplication::UnicodeUTF8));
        pushButton_23->setText(QApplication::translate("MainWindow", "_", 0, QApplication::UnicodeUTF8));
        b0->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        pushButton_25->setText(QApplication::translate("MainWindow", "<", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
