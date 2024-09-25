#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_connect_clicked();
    void on_clear_clicked();
    void on_sendButton_clicked();
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray data;
    quint32 nextBlockSize;
    const QString MESSAGE = "message";
    void SendToServer(QString path);
    void AddLog(QString str);
    void pathMessage(QDataStream* out);
    void end_of_program();
public slots:
    void slotReadyRead();
};

#endif // MAINWINDOW_H
