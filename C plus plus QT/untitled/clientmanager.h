#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <iostream>
#include <QTcpSocket>

class MainWindow;

class ClientManager
{
public:

    ClientManager(MainWindow* mainWindow);
    void connect(const QString& ip, const int port);
    void diconnect();
    void send(const QByteArray& data);
private:
     QTcpSocket* socket;
     MainWindow* mainWindow;
};

#endif // CLIENTMANAGER_H
