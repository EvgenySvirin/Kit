
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
    /*
     * Аварийно завершается при закрытии программы из-за этого потока
     * Не могу прописать почему-то деструктор ClientManagerа
     * Qt ругается нечитабельными символами, установилась на винду как-то странно
     *
     */

    return a.exec();
}
