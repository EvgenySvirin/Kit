
#include "clientmanager.h"
#include "mainwindow.h"
#include <iostream>
#include <QThread>

#include <QApplication>

bool MainWindow::isDebug = true;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    ClientManager clientManager(&mainWindow);
    mainWindow.setClientManager(&clientManager);
    std::thread t(&ClientManager::startRecieving, &clientManager);
    return a.exec();
}
