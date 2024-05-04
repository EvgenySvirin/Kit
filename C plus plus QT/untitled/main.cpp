#include "clientmanager.h"
#include "mainwindow.h"
#include <iostream>
#include <QThread>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ClientManager clientManager(&w);
    w.setClientManager(&clientManager);

    return a.exec();
}
