#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <iostream>
#include <QTcpSocket>
#include <QAtomicInteger>
#include <QTimer>
#include <thread>

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
    void startManaging();
    void startRecieving();

signals:

public slots:
    void connected();
    void disconnected();
    void errorSocket(QAbstractSocket::SocketError error);
    void checkIfConnected();
private:
     const int messageSizeLimit = 200;
     QAtomicInt::QAtomicInteger programIsEnded = false;
     QAtomicInt::QAtomicInteger isConnected = false;
     QTcpSocket* socket = nullptr;
     MainWindow* mainWindow = nullptr;
     QTimer* timer = nullptr;
     std::thread* recieverThread = nullptr;
};

#endif // CLIENTMANAGER_H
