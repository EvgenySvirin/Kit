#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <iostream>
#include <QTcpSocket>
#include <QAtomicInteger>

class MainWindow;

class ClientManager : public QObject
{
    Q_OBJECT
public:

    ClientManager(MainWindow* mainWindow);
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

private:
     QAtomicInt::QAtomicInteger isConnected = false;
     QTcpSocket* socket = nullptr;
     MainWindow* mainWindow = nullptr;
};

#endif // CLIENTMANAGER_H
