#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <iostream>
#include <QTcpSocket>
#include <QAtomicInteger>
#include <mutex>
#include <QTimer>

class MainWindow;

class ClientManager : public QObject
{
    Q_OBJECT
public:

    ClientManager(MainWindow* mainWindow);
    ~ClientManager();
    void connect(const QString& ip, const int port);
    void disconnect();
    bool send(const QByteArray& data);
    void writeEvent(const QString& eventMessage) const;
    void startRecieving();
signals:

public slots:
    void connected();
    void disconnected();
    void errorSocket(QAbstractSocket::SocketError error);
    void checkIfConnected();
private:
     QAtomicInt::QAtomicInteger programIsEnded = false;
     QAtomicInt::QAtomicInteger isConnected = false;
     QTcpSocket* socket = nullptr;
     MainWindow* mainWindow = nullptr;
     QTimer* timer = nullptr;
     std::thread* recieverThread = nullptr;
};

#endif // CLIENTMANAGER_H
