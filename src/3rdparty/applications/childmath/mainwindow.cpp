#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f) :
    QMainWindow(parent, f),
    ui(new Ui::MainWindow),
    okResults(0)
{
    ui->setupUi(this);
    newProblem();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newProblem()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    int a = qrand() % 10;
    int b = qrand() % 10;
    int op = qrand() % 2;
    if(op == 0)
    {
        ui->label->setText(QString::number(a) + "+" + QString::number(b) + "=");
        result = a + b;
    }
    else if(op == 1)
    {
        if(a < b)
        {
            int tmp = a;
            a = b;
            b = tmp;
        }
        ui->label->setText(QString::number(a) + "-" + QString::number(b) + "=");
        result = a - b;
    }
    input = 0;
}

void MainWindow::bClick(int num)
{
    QString text = ui->label->text() + QString::number(num);
    input = input * 10 + num;
    if(result >= 10 && input <= 9)
    {
        ui->label->setText(text);
        return;
    }

    if(input == result)
    {
        text += " :-)";
        okResults++;
    }
    else
    {
        text += " :-(";
    }
    ui->label->setText(text);

    int progress = ui->progressBar->value() + 1;
    if(progress == 10)
    {
        progress = 0;
        QMessageBox::information(this, "", ":-)  " + QString::number(okResults));
        okResults = 0;
    }
    ui->progressBar->setValue(progress);

    QTimer::singleShot(1000, this, SLOT(newProblem()));
}

void MainWindow::on_b0_clicked()
{
    bClick(0);
}

void MainWindow::on_b1_clicked()
{
    bClick(1);
}

void MainWindow::on_b2_clicked()
{
    bClick(2);
}

void MainWindow::on_b3_clicked()
{
    bClick(3);
}

void MainWindow::on_b4_clicked()
{
    bClick(4);
}

void MainWindow::on_b5_clicked()
{
    bClick(5);
}

void MainWindow::on_b6_clicked()
{
    bClick(6);
}

void MainWindow::on_b7_clicked()
{
    bClick(7);
}

void MainWindow::on_b8_clicked()
{
    bClick(8);
}

void MainWindow::on_b9_clicked()
{
    bClick(9);
}
