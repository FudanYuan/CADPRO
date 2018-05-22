/********************************************************************************
** Form generated from reading UI file 'sketch.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKETCH_H
#define UI_SKETCH_H

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

class Ui_Sketch
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *Sketch)
    {
        if (Sketch->objectName().isEmpty())
            Sketch->setObjectName(QStringLiteral("Sketch"));
        Sketch->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Sketch->sizePolicy().hasHeightForWidth());
        Sketch->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(Sketch);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Sketch->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Sketch);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 25));
        Sketch->setMenuBar(menuBar);
        statusBar = new QStatusBar(Sketch);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Sketch->setStatusBar(statusBar);
        mainToolBar = new QToolBar(Sketch);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Sketch->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(Sketch);

        QMetaObject::connectSlotsByName(Sketch);
    } // setupUi

    void retranslateUi(QMainWindow *Sketch)
    {
        Sketch->setWindowTitle(QApplication::translate("Sketch", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Sketch: public Ui_Sketch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKETCH_H
