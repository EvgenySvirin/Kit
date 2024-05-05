#include "clientmanager.h"
#include "mainwindow.h"
#include <QDebug>
#include <thread>
#include <QTimer>

class MainWindow;
ClientManager::ClientManager(MainWindow* mainWindow) {
   this->mainWindow = mainWindow;
   isConnected = false;
   socket = nullptr;
   recieverThread = new std::thread (&ClientManager::startRecieving, this);
   recieverThread->detach();
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
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkIfConnected()));
    timer->start(1000);
}

void ClientManager::checkIfConnected() {
    timer->stop();
    delete timer;
    timer = nullptr;
    if (!isConnected) {
        if (socket) {
            socket->close();
            delete socket;
            socket = nullptr;
            writeEvent("No connection\n");
        }
    }
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
    const std::string CLOSE = "CLOSE";
    while (!programIsEnded) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (isConnected && socket) {
            auto bytes = socket->read(200);
            if (bytes.length() != 0) {
                auto message = bytes.toStdString();
                auto it = message.find(CLOSE);
                if (it != std::string::npos) {
                    message.erase(it, CLOSE.length());
                    mainWindow->writeToChat(message);
                    if (isConnected) {
                        QMetaObject::invokeMethod(this, "disconnected", Qt::QueuedConnection);
                    }
                } else {
                    mainWindow->writeToChat(message);
                }
            }
        }
    }
}

bool ClientManager::send(const QByteArray& data) {
    try {
        if (MainWindow::isDebug) {
            qDebug() << "Entering send\n";
        }
        if (200 < data.size()) {
            writeEvent("Message is too large, more than 200 symbols\n");
            return false;
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

ClientManager::~ClientManager() {
    qDebug() << "called dest";
    programIsEnded = true;
    if (timer) {
        delete timer;
        timer = nullptr;
    }
    if (socket) {
        socket->close();
        delete socket;
        socket = nullptr;
    }
}
