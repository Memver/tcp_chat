#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QFontMetrics>
#include "crc32.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this,
            &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket,
            &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendToServer(QString path)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    if(path == MESSAGE){
        pathMessage(&out);
    }
    socket->write(data);
}

void MainWindow::AddLog(QString str)
{
    ui->textBrowser->append(QTime::currentTime().toString() + ": " + str);
}

void MainWindow::pathMessage(QDataStream* out)
{
    QString message = ui->textEdit->toPlainText();
    AddLog("Client: " + message);

    crc32 crc32;
    unsigned int sum = crc32.CRC32_count_qstring(message);

    *out << quint32(0) << quint32(0) << MESSAGE; // nulls and contents
    *out << message;
    out->device()->seek(0);
    *out << quint32(data.size() - sizeof(quint32)); // size
    *out << quint32(sum); // sum
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status() == QDataStream::Ok){
        while(true)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            QString str;
            in >> str;
            AddLog("Server: " + str);
            if(str == "КОНЕЦ"){
                end_of_program();
            }
            nextBlockSize = 0;
            break;
        }
    }
    else{
        qDebug() << "read error";
    }
}

void MainWindow::end_of_program(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Dialogue"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->textBrowser->toPlainText();
            file.close();
            AddLog("Dialogue saved to: " + fileName);
        } else {
            qDebug() << "Failed to save the file.";
        }
    }
    socket->disconnectFromHost();
    AddLog("Disconnected from server");
}

void MainWindow::on_clear_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_connect_clicked()
{
    socket->connectToHost("127.0.0.1", 80);
}

void MainWindow::on_sendButton_clicked()
{
    SendToServer(MESSAGE);
}
