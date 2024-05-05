#include "clientmanager.h"
#include "mainwindow.h"
#include <QDebug>
#include <thread>

class MainWindow;
ClientManager::ClientManager(MainWindow* mainWindow) {
   this->mainWindow = mainWindow;
   isConnected = false;
   socket = nullptr;
}

void ClientManager::connect(const QString& ip, const int port) {

    if (socket) {
        disconnected();
    }
    socket = new QTcpSocket();

    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                     SLOT(errorSocket(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
}

void ClientManager::disconnect() {
    disconnected();
}

void ClientManager::disconnected() {
    if (MainWindow::isDebug) {
         qDebug() << "Disconnected slot is called";
    }
    if (!isConnected) {
        return;
    }
    isConnected = false;
    if (socket) {
        socket->close();
        delete socket;
        socket = nullptr;
        writeEvent("Disconnected\n");
    }
    isConnected = false;
    mainWindow->toggleConnectionStatus(false);
}

void ClientManager::connected() {
    writeEvent("Connected\n");
    isConnected = true;
    mainWindow->toggleConnectionStatus(true);
}

void ClientManager::errorSocket(QAbstractSocket::SocketError error) {
    qDebug() << error;
}

void ClientManager::startRecieving() {
    while (true) {
        if (isConnected && socket) {
            auto bytes = socket->readAll();

            if (bytes.length() != 0) {
                auto qMessage = QString::fromStdString(bytes.toStdString());
                if (qMessage.contains("CLOSE")) {
                    qDebug() << "Close called" << "\n";
                    mainWindow->writeEvent("CLOSE");
                    QMetaObject::invokeMethod(this, "disconnected", Qt::QueuedConnection);
                } else if (qMessage.contains("Server:")) {
                    mainWindow->writeToChat(qMessage.toStdString());
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}

bool ClientManager::send(const QByteArray& data) {
    try {
        if (MainWindow::isDebug) {
            qDebug() << "Entering send\n";
        }
        socket->write(data);
        socket->flush();
        writeEvent("Sent message to server\n");
        return true;
    } catch (std::exception e) {
        qDebug() << "Exception on send:" << e.what();
        disconnected();
        return false;
    }
}

void ClientManager::writeEvent(const QString &eventMessage) const {
    mainWindow->writeEvent(eventMessage);
}
