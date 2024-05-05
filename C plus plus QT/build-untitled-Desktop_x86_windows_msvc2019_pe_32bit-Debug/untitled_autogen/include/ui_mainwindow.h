/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QLineEdit *textIP;
    QLineEdit *textPort;
    QLabel *label;
    QLabel *label_2;
    QTextEdit *textEvents;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton_2;
    QLabel *label_5;
    QTextEdit *messageText;
    QPushButton *pushButton_3;
    QTextEdit *textChat;
    QCheckBox *checkBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1047, 606);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(930, 20, 93, 28));
        textIP = new QLineEdit(centralwidget);
        textIP->setObjectName(QString::fromUtf8("textIP"));
        textIP->setGeometry(QRect(650, 20, 113, 22));
        textPort = new QLineEdit(centralwidget);
        textPort->setObjectName(QString::fromUtf8("textPort"));
        textPort->setGeometry(QRect(810, 20, 113, 22));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(620, 20, 21, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(770, 20, 55, 16));
        textEvents = new QTextEdit(centralwidget);
        textEvents->setObjectName(QString::fromUtf8("textEvents"));
        textEvents->setGeometry(QRect(180, 90, 361, 151));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 90, 55, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 310, 55, 16));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(930, 60, 93, 28));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(600, 310, 55, 16));
        messageText = new QTextEdit(centralwidget);
        messageText->setObjectName(QString::fromUtf8("messageText"));
        messageText->setGeometry(QRect(670, 310, 251, 151));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(930, 430, 93, 28));
        textChat = new QTextEdit(centralwidget);
        textChat->setObjectName(QString::fromUtf8("textChat"));
        textChat->setGeometry(QRect(180, 310, 361, 151));
        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(680, 100, 141, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1047, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        textIP->setText(QApplication::translate("MainWindow", "127.0.0.1", nullptr));
        textPort->setText(QApplication::translate("MainWindow", "9000", nullptr));
        label->setText(QApplication::translate("MainWindow", "IP:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Port:", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Events:", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Chat:", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Message:", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Send", nullptr));
        checkBox->setText(QApplication::translate("MainWindow", "Connected to server", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
