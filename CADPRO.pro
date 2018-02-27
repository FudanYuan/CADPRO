#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T16:36:49
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CADPRO
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += dxflib

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mydocktitlebar.cpp \
    project.cpp \
    dxflib/dl_writer_ascii.cpp \
    dxflib/dl_dxf.cpp \
    dxffilter.cpp \
    configure.cpp \
    configuredialog.cpp \
    customwidget.cpp \
    view.cpp \
    scene.cpp \
    shape.cpp \
    line.cpp \
    rect.cpp \
    ellipse.cpp \
    circle.cpp \
    arc.cpp

HEADERS += \
        debug.h \
        mainwindow.h \
    mydocktitlebar.h \
    project.h \
    dxflib/dl_writer.h \
    dxflib/dl_writer_ascii.h \
    dxflib/dl_global.h \
    dxflib/dl_extrusion.h \
    dxflib/dl_exception.h \
    dxflib/dl_entities.h \
    dxflib/dl_dxf.h \
    dxflib/dl_creationinterface.h \
    dxflib/dl_creationadapter.h \
    dxflib/dl_codes.h \
    dxflib/dl_attributes.h \
    dxffilter.h \
    mydxfentities.h \
    configure.h \
    configuredialog.h \
    customwidget.h \
    view.h \
    scene.h \
    shape.h \
    line.h \
    rect.h \
    ellipse.h \
    circle.h \
    arc.h

FORMS += \
        mainwindow.ui \
    configuredialog.ui
