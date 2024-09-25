#include <QApplication>

#include "mainwindow.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Server s(&w);
    w.server = &s;
    w.show();
    return a.exec();
}
