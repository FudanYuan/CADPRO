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
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Nest
{
public:
    QWidget *centralwidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

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
        toolBar = new QToolBar(Nest);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        Nest->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(Nest);

        QMetaObject::connectSlotsByName(Nest);
    } // setupUi

    void retranslateUi(QMainWindow *Nest)
    {
        Nest->setWindowTitle(QApplication::translate("Nest", "MainWindow", nullptr));
        toolBar->setWindowTitle(QApplication::translate("Nest", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Nest: public Ui_Nest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEST_H
