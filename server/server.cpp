#include "QFile"
#include "server.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crc32.h"


Server::Server(MainWindow* w) {
    this->w = w;
    if(this->listen(QHostAddress::Any, 80)) {
        w->AddLog("QHostAddress listen");
    } else {
        w->AddLog("error QHostAddress listen");
    }
    size = 0;
}

Server::~Server()
{
    for(int i = 0; i < sockets.size(); i++){
        delete sockets[i];
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this,
            &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket,
            &QTcpSocket::deleteLater);
    sockets.push_back(socket);
    w->AddLog("client connect with socketDescriptor = " +
              QString::number(socketDescriptor));
    SendToClient("success connect " +
                 QString::number(socketDescriptor));
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
        while(true){
            if(size == 0){
                qDebug() << "nexBlockSize = 0";
                if(socket->bytesAvailable() <
                    quint32(sizeof(size)))
                {
                    qDebug() << "Data < sizeof(size),break";
                    break;
                }
                in >> size;
                qDebug() << "size = " + QString::number(size);
                if(size > max_size){
                    qDebug() << "error: size message Too Large";
                    SendToClient("error: size message Too Large.max_size =" + QString::number(max_size));
                    socket->deleteLater();
                    break;
                }
            }
            if(socket->bytesAvailable() < size){
                qDebug() << "error: Data not full, break.bytesAvailable = " + QString::number(socket->bytesAvailable());
                break;
            }
            qDebug() << "bytesAvailable = " +
                            QString::number(socket->bytesAvailable());
            quint32 sum;
            QString path;
            in >> sum >> path;
            if(path == MESSAGE){
                pathMessage(sum, &in);
            }
            else{
                qDebug() << "Undefined path";
            }
            size = 0;
            break;
        }
    }
    else{
        qDebug() << "DataStream error";
    }
}

void Server::SendToClient(QString str)
{
    w->AddLog("Server: " + str);

    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << quint32(0) << str;
    out.device()->seek(0);
    out << quint32(data.size() - sizeof(quint32));
    socket->write(data);
}

void Server::pathMessage(quint32 sum_right, QDataStream* in)
{
    QString message;
    *in >> message;

    crc32 crc32;
    quint32 sum_cur = quint32(crc32.CRC32_count_qstring(message));

    if(sum_cur == sum_right){
        w->AddLog("Client: " + message);
    }else{
        qDebug("error: wrong sum");
    }
}
