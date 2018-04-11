/********************************************************************************
** Form generated from reading UI file 'nest.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEST_H
#define UI_NEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Nest
{
public:
    QWidget *centralwidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Nest)
    {
        if (Nest->objectName().isEmpty())
            Nest->setObjectName(QStringLiteral("Nest"));
        Nest->resize(800, 600);
        centralwidget = new QWidget(Nest);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        Nest->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(Nest);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        Nest->setMenuBar(menuBar);
        statusBar = new QStatusBar(Nest);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Nest->setStatusBar(statusBar);

        retranslateUi(Nest);

        QMetaObject::connectSlotsByName(Nest);
    } // setupUi

    void retranslateUi(QMainWindow *Nest)
    {
        Nest->setWindowTitle(QApplication::translate("Nest", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Nest: public Ui_Nest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEST_H
