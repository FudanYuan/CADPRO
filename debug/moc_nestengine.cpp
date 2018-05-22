/****************************************************************************
** Meta object code from reading C++ file 'nestengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cad/nestengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nestengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NestEngine_t {
    QByteArrayData data[22];
    char stringdata0[348];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NestEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NestEngine_t qt_meta_stringdata_NestEngine = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NestEngine"
QT_MOC_LITERAL(1, 11, 15), // "NestMixingTypes"
QT_MOC_LITERAL(2, 27, 8), // "NoMixing"
QT_MOC_LITERAL(3, 36, 15), // "TailPieceMixing"
QT_MOC_LITERAL(4, 52, 14), // "TailLineMixing"
QT_MOC_LITERAL(5, 67, 18), // "SameTypeSizeMixing"
QT_MOC_LITERAL(6, 86, 9), // "AllMixing"
QT_MOC_LITERAL(7, 96, 24), // "NestAdaptiveSpacingTypes"
QT_MOC_LITERAL(8, 121, 17), // "NoAdaptiveSpacing"
QT_MOC_LITERAL(9, 139, 25), // "HorizontalAdaptiveSpacing"
QT_MOC_LITERAL(10, 165, 23), // "VerticalAdaptiveSpacing"
QT_MOC_LITERAL(11, 189, 18), // "AllAdaptiveSpacing"
QT_MOC_LITERAL(12, 208, 16), // "NestOrientations"
QT_MOC_LITERAL(13, 225, 14), // "HorizontalNest"
QT_MOC_LITERAL(14, 240, 12), // "VerticalNest"
QT_MOC_LITERAL(15, 253, 18), // "AllOrientationNest"
QT_MOC_LITERAL(16, 272, 19), // "NestEngineStrategys"
QT_MOC_LITERAL(17, 292, 10), // "NoStrategy"
QT_MOC_LITERAL(18, 303, 13), // "LeftRightTurn"
QT_MOC_LITERAL(19, 317, 8), // "SizeDown"
QT_MOC_LITERAL(20, 326, 12), // "AllStrategys"
QT_MOC_LITERAL(21, 339, 8) // "Reserved"

    },
    "NestEngine\0NestMixingTypes\0NoMixing\0"
    "TailPieceMixing\0TailLineMixing\0"
    "SameTypeSizeMixing\0AllMixing\0"
    "NestAdaptiveSpacingTypes\0NoAdaptiveSpacing\0"
    "HorizontalAdaptiveSpacing\0"
    "VerticalAdaptiveSpacing\0AllAdaptiveSpacing\0"
    "NestOrientations\0HorizontalNest\0"
    "VerticalNest\0AllOrientationNest\0"
    "NestEngineStrategys\0NoStrategy\0"
    "LeftRightTurn\0SizeDown\0AllStrategys\0"
    "Reserved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NestEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       4,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x1,    5,   30,
       7, 0x1,    4,   40,
      12, 0x1,    3,   48,
      16, 0x1,    5,   54,

 // enum data: key, value
       2, uint(NestEngine::NoMixing),
       3, uint(NestEngine::TailPieceMixing),
       4, uint(NestEngine::TailLineMixing),
       5, uint(NestEngine::SameTypeSizeMixing),
       6, uint(NestEngine::AllMixing),
       8, uint(NestEngine::NoAdaptiveSpacing),
       9, uint(NestEngine::HorizontalAdaptiveSpacing),
      10, uint(NestEngine::VerticalAdaptiveSpacing),
      11, uint(NestEngine::AllAdaptiveSpacing),
      13, uint(NestEngine::HorizontalNest),
      14, uint(NestEngine::VerticalNest),
      15, uint(NestEngine::AllOrientationNest),
      17, uint(NestEngine::NoStrategy),
      18, uint(NestEngine::LeftRightTurn),
      19, uint(NestEngine::SizeDown),
      20, uint(NestEngine::AllStrategys),
      21, uint(NestEngine::Reserved),

       0        // eod
};

void NestEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NestEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NestEngine.data,
      qt_meta_data_NestEngine,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NestEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NestEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NestEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NestEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
