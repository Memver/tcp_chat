#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class MainWindow;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(MainWindow* w);
    ~Server();
    void SendToClient(QString str);
private:
    QTcpSocket *socket;
    MainWindow* w;
    QVector <QTcpSocket*> sockets;
    QByteArray data;
    quint32 size;
    const QString MESSAGE = "message";
    const quint32 max_size = (1 << 16) - 100; // for example
    void pathMessage(quint32 sum_right, QDataStream* in);
public slots:
    void incomingConnection(qintptr socketDescription);
    void slotReadyRead();
};

#endif // SERVER_H
