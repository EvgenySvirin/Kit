#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setClientManager(ClientManager* clientManager) {
    this->clientManager = clientManager;
}

void MainWindow::on_pushButton_clicked() {
    auto ip = ui->textIP->text();
    QHostAddress address(ip);
    if (address.protocol() != QAbstractSocket::IPv4Protocol &&
            address.protocol() != QAbstractSocket::IPv6Protocol) {
        writeEvent("Wrong ip\n");
        return;
    }
    bool success = false;
    auto port = ui->textPort->text().toInt(&success);
    if (!success) {
        writeEvent("Wrong port\n");
        return;
    }
    clientManager->connect(ip, port);
}

void MainWindow::on_pushButton_2_clicked() {
    clientManager->disconnect();
}

void MainWindow::on_pushButton_3_clicked() {
    auto text = "Client:" + ui->messageText->toPlainText().toStdString() + '\n';

    if (clientManager->send(QByteArray(text.c_str(), text.length()))) {
        ui->textChat->append(QString::fromStdString(text));
    }
}

void MainWindow::writeEvent(const QString &eventMessage) {
    ui->textEvents->append(eventMessage);
}

void MainWindow::toggleConnectionStatus(const bool& isConnected) {
    ui->checkBox->setChecked(isConnected);
}

void MainWindow::writeToChat(std::string message) {
    ui->textChat->append(QString::fromStdString(message));
}

