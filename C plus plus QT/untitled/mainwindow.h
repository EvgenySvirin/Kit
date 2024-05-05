#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "clientmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static bool isDebug;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setClientManager(ClientManager* clientManager);

    void writeEvent(const QString& eventMessage);

    void toggleConnectionStatus(const bool& isConnected);

    void writeToChat(const std::string& message);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    ClientManager* clientManager = nullptr;

};
#endif // MAINWINDOW_H
