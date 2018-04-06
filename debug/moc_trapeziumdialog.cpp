/****************************************************************************
** Meta object code from reading C++ file 'trapeziumdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VSI/item/trapeziumdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trapeziumdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrapeziumDialog_t {
    QByteArrayData data[10];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrapeziumDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrapeziumDialog_t qt_meta_stringdata_TrapeziumDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TrapeziumDialog"
QT_MOC_LITERAL(1, 16, 11), // "onclickedok"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 15), // "onclickedcancel"
QT_MOC_LITERAL(4, 45, 14), // "onalpha1change"
QT_MOC_LITERAL(5, 60, 14), // "onalpha2change"
QT_MOC_LITERAL(6, 75, 9), // "onHchange"
QT_MOC_LITERAL(7, 85, 17), // "ontopLengthchange"
QT_MOC_LITERAL(8, 103, 16), // "onlineTypechange"
QT_MOC_LITERAL(9, 120, 19) // "onchooseCheckchange"

    },
    "TrapeziumDialog\0onclickedok\0\0"
    "onclickedcancel\0onalpha1change\0"
    "onalpha2change\0onHchange\0ontopLengthchange\0"
    "onlineTypechange\0onchooseCheckchange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrapeziumDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TrapeziumDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrapeziumDialog *_t = static_cast<TrapeziumDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onclickedok(); break;
        case 1: _t->onclickedcancel(); break;
        case 2: _t->onalpha1change(); break;
        case 3: _t->onalpha2change(); break;
        case 4: _t->onHchange(); break;
        case 5: _t->ontopLengthchange(); break;
        case 6: _t->onlineTypechange(); break;
        case 7: _t->onchooseCheckchange(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject TrapeziumDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TrapeziumDialog.data,
      qt_meta_data_TrapeziumDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrapeziumDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrapeziumDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrapeziumDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TrapeziumDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
