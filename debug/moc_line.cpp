/****************************************************************************
** Meta object code from reading C++ file 'line.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VSI/item/line.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'line.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Line_t {
    QByteArrayData data[8];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Line_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Line_t qt_meta_stringdata_Line = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Line"
QT_MOC_LITERAL(1, 5, 6), // "select"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 5), // "Line*"
QT_MOC_LITERAL(4, 19, 4), // "line"
QT_MOC_LITERAL(5, 24, 22), // "onSceneMoveableChanged"
QT_MOC_LITERAL(6, 47, 8), // "moveable"
QT_MOC_LITERAL(7, 56, 10) // "typechange"

    },
    "Line\0select\0\0Line*\0line\0onSceneMoveableChanged\0"
    "moveable\0typechange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Line[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   32,    2, 0x0a /* Public */,
       7,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,

       0        // eod
};

void Line::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Line *_t = static_cast<Line *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->select((*reinterpret_cast< Line*(*)>(_a[1]))); break;
        case 1: _t->onSceneMoveableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->typechange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Line* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Line::*_t)(Line * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Line::select)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Line::staticMetaObject = {
    { &Shape::staticMetaObject, qt_meta_stringdata_Line.data,
      qt_meta_data_Line,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Line::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Line::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Line.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsLineItem"))
        return static_cast< QGraphicsLineItem*>(this);
    return Shape::qt_metacast(_clname);
}

int Line::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Shape::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Line::select(Line * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
