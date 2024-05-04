#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setClientManager(ClientManager* clientManager) {
    this->clientManager = clientManager;
}


void MainWindow::on_pushButton_clicked()
{
    auto ip = ui->textIP->text();
    auto port = ui->textPort->text();
    clientManager->connect(ip, port.toInt());
}


void MainWindow::on_pushButton_2_clicked()
{
    clientManager->diconnect();
}

void MainWindow::on_pushButton_3_clicked()
{
    auto text = ui->messageText->toPlainText().toStdString();
    clientManager->send(QByteArray(text.c_str(), text.length()));
}

