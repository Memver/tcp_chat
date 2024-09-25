#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QFontMetrics>

#include "mainwindow.h"
#include "server.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddLog(QString str)
{
    ui->textBrowser->append(QTime::currentTime().toString() + ": " + str);
}

void MainWindow::on_clear_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_sendButton_clicked()
{
    server->SendToClient(ui->textEdit->toPlainText());
}
