#ifndef QX_H
#define QX_H

#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QApplication>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDesktopWidget>
#ifdef QT_QWS_FICGTA01
#include <QtopiaApplication>
#endif

class QX : public QWidget
{
    Q_OBJECT

public:
    QX(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QX();

private:
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QLabel* label;
    QLineEdit *lineEdit;
    QPushButton* bOk;
    QPushButton* bBack;
    QProcess* process;
    bool fullScreen;
    bool is_fullscreen;

protected:
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
    void focusInEvent(QFocusEvent *);
    void enableFullscreen();
	
	bool event(QEvent *event);
    void enterFullScreen();
    void exitFullScreen();

private slots:
    void okClicked();
    void backClicked();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QX_H
