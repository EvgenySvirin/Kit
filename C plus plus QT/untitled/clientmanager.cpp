#include "clientmanager.h"
#include "mainwindow.h"

#include <QDebug>


class MainWindow;
ClientManager::ClientManager(MainWindow* mainWindow) {
   this->mainWindow = mainWindow;
   isConnected = false;
   socket = new QTcpSocket();
   timer = new QTimer();
   QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkIfConnected()));
}

void ClientManager::startManaging() {
    recieverThread = new std::thread (&ClientManager::startRecieving, this);
    recieverThread->detach();
}

void ClientManager::connect(const QString& ip, const int port) {
    if (socket) {
        disconnected();
    }
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                     SLOT(errorSocket(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
    timer->start(1000);
}

void ClientManager::checkIfConnected() {
    timer->stop();
    if (!isConnected) {
        if (socket) {
            socket->close();
            writeEvent("No connection\n");
        }
    }
}
void ClientManager::disconnect() {
    disconnected();
}

void ClientManager::disconnected() {
    if (MainWindow::isDebug) {
         qDebug() << "Disconnected slot called";
    }
    if (!isConnected) {
        return;
    }
    isConnected = false;
    socket->close();
    writeEvent("Disconnected\n");
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
        if (isConnected && socket && socket->isReadable()) {
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
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

bool ClientManager::send(const QByteArray& data) {
    try {
        if (MainWindow::isDebug) {
            qDebug() << "Entering send\n";
        }
        if (!isConnected) {
            writeEvent("No connection\n");
            return false;
        }
        if (messageSizeLimit < data.size()) {
            writeEvent("Message is too large\n");
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
    programIsEnded = true;
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    if (socket) {
        socket->close();
        delete socket;
        socket = nullptr;
    }
    if (recieverThread) {
        delete recieverThread;
    }
}
