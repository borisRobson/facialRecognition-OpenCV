/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri May 27 14:09:43 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGroupBox *gbAddUsers;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *txtUser;
    QLineEdit *txtUserName;
    QPushButton *btnAddUser;
    QGroupBox *gbDetcetUsers;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnDetect;
    QLabel *txtDetected;
    QLineEdit *txtDetectedUser;
    QGroupBox *gbMessages;
    QLineEdit *txtMessages;
    QMenuBar *menuBar;
    QMenu *menuFacial_Recognition;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(685, 419);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 621, 321));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gbAddUsers = new QGroupBox(widget);
        gbAddUsers->setObjectName(QString::fromUtf8("gbAddUsers"));
        widget1 = new QWidget(gbAddUsers);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(10, 40, 601, 29));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        txtUser = new QLabel(widget1);
        txtUser->setObjectName(QString::fromUtf8("txtUser"));

        horizontalLayout_2->addWidget(txtUser);

        txtUserName = new QLineEdit(widget1);
        txtUserName->setObjectName(QString::fromUtf8("txtUserName"));

        horizontalLayout_2->addWidget(txtUserName);

        btnAddUser = new QPushButton(widget1);
        btnAddUser->setObjectName(QString::fromUtf8("btnAddUser"));

        horizontalLayout_2->addWidget(btnAddUser);


        verticalLayout->addWidget(gbAddUsers);

        gbDetcetUsers = new QGroupBox(widget);
        gbDetcetUsers->setObjectName(QString::fromUtf8("gbDetcetUsers"));
        widget2 = new QWidget(gbDetcetUsers);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        widget2->setGeometry(QRect(10, 40, 591, 29));
        horizontalLayout = new QHBoxLayout(widget2);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnDetect = new QPushButton(widget2);
        btnDetect->setObjectName(QString::fromUtf8("btnDetect"));

        horizontalLayout->addWidget(btnDetect);

        txtDetected = new QLabel(widget2);
        txtDetected->setObjectName(QString::fromUtf8("txtDetected"));

        horizontalLayout->addWidget(txtDetected);

        txtDetectedUser = new QLineEdit(widget2);
        txtDetectedUser->setObjectName(QString::fromUtf8("txtDetectedUser"));

        horizontalLayout->addWidget(txtDetectedUser);


        verticalLayout->addWidget(gbDetcetUsers);

        gbMessages = new QGroupBox(widget);
        gbMessages->setObjectName(QString::fromUtf8("gbMessages"));
        txtMessages = new QLineEdit(gbMessages);
        txtMessages->setObjectName(QString::fromUtf8("txtMessages"));
        txtMessages->setGeometry(QRect(0, 20, 611, 71));

        verticalLayout->addWidget(gbMessages);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 685, 25));
        menuFacial_Recognition = new QMenu(menuBar);
        menuFacial_Recognition->setObjectName(QString::fromUtf8("menuFacial_Recognition"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFacial_Recognition->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        gbAddUsers->setTitle(QApplication::translate("MainWindow", "Add Users:", 0, QApplication::UnicodeUTF8));
        txtUser->setText(QApplication::translate("MainWindow", "Name: ", 0, QApplication::UnicodeUTF8));
        btnAddUser->setText(QApplication::translate("MainWindow", "Add User", 0, QApplication::UnicodeUTF8));
        gbDetcetUsers->setTitle(QApplication::translate("MainWindow", "Detect Users:", 0, QApplication::UnicodeUTF8));
        btnDetect->setText(QApplication::translate("MainWindow", "Detect", 0, QApplication::UnicodeUTF8));
        txtDetected->setText(QApplication::translate("MainWindow", "Detected:", 0, QApplication::UnicodeUTF8));
        gbMessages->setTitle(QApplication::translate("MainWindow", "Messages:", 0, QApplication::UnicodeUTF8));
        menuFacial_Recognition->setTitle(QApplication::translate("MainWindow", "Facial Recognition", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
