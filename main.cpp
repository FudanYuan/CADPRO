#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "view.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Jeremy");
    QApplication::setApplicationName("CADPRO");
    MainWindow w;
    w.show();

    return a.exec();
}
