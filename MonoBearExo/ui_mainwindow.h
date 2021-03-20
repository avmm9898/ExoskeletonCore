/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *Labelbtn;
    QLabel *labelpos;
    QSpinBox *spinBox;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout;
    QLabel *label_max;
    QPushButton *btn_up;
    QPushButton *btn_down;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Labelbtn = new QLabel(centralWidget);
        Labelbtn->setObjectName(QStringLiteral("Labelbtn"));

        horizontalLayout->addWidget(Labelbtn);

        labelpos = new QLabel(centralWidget);
        labelpos->setObjectName(QStringLiteral("labelpos"));

        horizontalLayout->addWidget(labelpos);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        horizontalLayout->addWidget(spinBox);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_max = new QLabel(centralWidget);
        label_max->setObjectName(QStringLiteral("label_max"));

        verticalLayout->addWidget(label_max);

        btn_up = new QPushButton(centralWidget);
        btn_up->setObjectName(QStringLiteral("btn_up"));

        verticalLayout->addWidget(btn_up);

        btn_down = new QPushButton(centralWidget);
        btn_down->setObjectName(QStringLiteral("btn_down"));

        verticalLayout->addWidget(btn_down);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 34));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        Labelbtn->setText(QApplication::translate("MainWindow", "BTN", nullptr));
        labelpos->setText(QApplication::translate("MainWindow", "Pos", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "set", nullptr));
        label_max->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        btn_up->setText(QApplication::translate("MainWindow", "UP", nullptr));
        btn_down->setText(QApplication::translate("MainWindow", "DOWN", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
