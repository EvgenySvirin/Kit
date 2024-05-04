#include "clientmanager.h"

ClientManager::ClientManager(MainWindow* mainWindow)
{
   this->mainWindow = mainWindow;
}

void ClientManager::connect(const QString& ip, const int port) {
    socket = new QTcpSocket();
    socket->connectToHost(ip, port);
}

void ClientManager::diconnect() {
    if (!socket) {
        return;
    }
    socket->close();
    delete socket;
    socket = nullptr;
}

void ClientManager::send(const QByteArray& data) {
    if (!socket) {
        return;
    }
    socket->write(data);
}
