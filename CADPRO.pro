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

INCLUDEPATH += cad
INCLUDEPATH += configure
INCLUDEPATH += common
INCLUDEPATH += dxf
INCLUDEPATH += dxf/dxflib
INCLUDEPATH += nestFile
INCLUDEPATH += VSI
INCLUDEPATH += VSI/item

SOURCES += \
    main.cpp \
    common/common.cpp \
    common/painter.cpp \
    common/customdocktitlebar.cpp \
    common/customwidget.cpp \
    common/GA.cpp \
    dxf/dxflib/dl_writer_ascii.cpp \
    dxf/dxflib/dl_dxf.cpp \
    dxf/dxffilter.cpp \
    configure/configure.cpp \
    configure/configuredialog.cpp \
    VSI/view.cpp \
    VSI/scene.cpp \
    VSI/item/shape.cpp \
    VSI/item/point.cpp \
    VSI/item/polyline.cpp \
    VSI/item/polygon.cpp \
    VSI/item/line.cpp \
    VSI/item/rect.cpp \
    VSI/item/ellipse.cpp \
    VSI/item/circle.cpp \
    VSI/item/arc.cpp \
    VSI/item/renderarea.cpp \
    VSI/item/trapezium.cpp \
    VSI/item/trapeziumdialog.cpp \
    VSI/item/polygondialog.cpp \
    VSI/item/eyelet.cpp \
    VSI/item/eyeletdialog.cpp \
    VSI/item/text.cpp \
    VSI/item/textdialog.cpp \
    VSI/item/itemproperties.cpp \
    VSI/item/insertoffsetdialog.cpp \
    cad/project.cpp \
    cad/sketch.cpp \
    cad/nest.cpp \
    cad/sheet.cpp \
    nestFile/nf_writer.cpp \
    nestFile/nf_tnf.cpp \
    nestFile/nf_reader.cpp \
    cad/rectnestengine.cpp

HEADERS += \
    common/debug.h \
    common/common.h \
    common/painter.h \
    common/customdocktitlebar.h \
    common/customwidget.h \
    common/GA.h \
    common/binarytree.h \
    common/ternarytree.h \
    dxf/dxflib/dl_writer.h \
    dxf/dxflib/dl_writer_ascii.h \
    dxf/dxflib/dl_global.h \
    dxf/dxflib/dl_extrusion.h \
    dxf/dxflib/dl_exception.h \
    dxf/dxflib/dl_entities.h \
    dxf/dxflib/dl_dxf.h \
    dxf/dxflib/dl_creationinterface.h \
    dxf/dxflib/dl_creationadapter.h \
    dxf/dxflib/dl_codes.h \
    dxf/dxflib/dl_attributes.h \
    dxf/dxflib/customentities.h \
    dxf/dxffilter.h \
    configure/configure.h \
    configure/configuredialog.h \
    VSI/view.h \
    VSI/scene.h \
    VSI/item/shape.h \
    VSI/item/point.h \
    VSI/item/line.h \
    VSI/item/polyline.h \
    VSI/item/polygon.h \
    VSI/item/rect.h \
    VSI/item/ellipse.h \
    VSI/item/circle.h \
    VSI/item/arc.h \
    VSI/item/renderarea.h \
    VSI/item/trapezium.h \
    VSI/item/trapeziumdialog.h \
    VSI/item/polygondialog.h \
    VSI/item/eyelet.h \
    VSI/item/eyeletdialog.h \
    VSI/item/text.h \
    VSI/item/textdialog.h \
    VSI/item/itemproperties.h \
    VSI/item/insertoffsetdialog.h \
    cad/project.h \
    cad/sketch.h \
    cad/nest.h \
    common/binaryfile.h \
    cad/sheet.h \
    nestFile/nf_global.h \
    nestFile/nf_writer.h \
    nestFile/nf_tnf.h \
    nestFile/nf_reader.h \
    nestFile/nf_struct.h \
    cad/rectnestengine.h

FORMS += \
    configure/configuredialog.ui \
    cad/sketch.ui \
    cad/nest.ui

TRANSLATIONS = CADPRO_en_CN.ts
