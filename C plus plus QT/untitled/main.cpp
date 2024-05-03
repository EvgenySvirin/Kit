#include "mainwindow.h"
#include <iostream>
#include <QThread>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QTcpSocket* socket;
    QByteArray data = "POPA"; // <-- fill with data

    while (true) {
        std::cout << "Entered";


        socket = new QTcpSocket(); // <-- needs to be a member variable: QTcpSocket * _pSocket;

        socket->connectToHost("127.0.0.1", 9052);
        if( socket->waitForConnected() ) {
            socket->write(data);
             std::cout << "sent1";
            break;
        }
        delete socket;
        std::cout << "left";
        QThread::sleep(10);
    }

    socket->write(data);
    std::cout << "sent2";
    socket->write(data);
    std::cout << "sent3";
    socket->write(data);
    std::cout << "sent4";
    socket->write(data);
    std::cout << "sent5";
    socket->write(data);
    std::cout << "sent6";
    /* really important */
    socket->flush();

    QThread::sleep(1000);

    //return a.exec();
}
