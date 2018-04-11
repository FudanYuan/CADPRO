/****************************************************************************
** Meta object code from reading C++ file 'sheet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cad/sheet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sheet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SheetDialog_t {
    QByteArrayData data[18];
    char stringdata0[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SheetDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SheetDialog_t qt_meta_stringdata_SheetDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SheetDialog"
QT_MOC_LITERAL(1, 12, 13), // "sheetSelected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 1), // "i"
QT_MOC_LITERAL(4, 29, 25), // "onStripConfigTableChanged"
QT_MOC_LITERAL(5, 55, 3), // "row"
QT_MOC_LITERAL(6, 59, 6), // "column"
QT_MOC_LITERAL(7, 66, 20), // "onDoubleStripChanged"
QT_MOC_LITERAL(8, 87, 6), // "checkd"
QT_MOC_LITERAL(9, 94, 21), // "onSheetTypeComChanged"
QT_MOC_LITERAL(10, 116, 5), // "index"
QT_MOC_LITERAL(11, 122, 21), // "onDialogButtonClicked"
QT_MOC_LITERAL(12, 144, 16), // "QAbstractButton*"
QT_MOC_LITERAL(13, 161, 6), // "button"
QT_MOC_LITERAL(14, 168, 15), // "onSheetSelected"
QT_MOC_LITERAL(15, 184, 13), // "onSheetInsert"
QT_MOC_LITERAL(16, 198, 13), // "onSheetRemove"
QT_MOC_LITERAL(17, 212, 11) // "onSheetEdit"

    },
    "SheetDialog\0sheetSelected\0\0i\0"
    "onStripConfigTableChanged\0row\0column\0"
    "onDoubleStripChanged\0checkd\0"
    "onSheetTypeComChanged\0index\0"
    "onDialogButtonClicked\0QAbstractButton*\0"
    "button\0onSheetSelected\0onSheetInsert\0"
    "onSheetRemove\0onSheetEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SheetDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   62,    2, 0x08 /* Private */,
       7,    1,   67,    2, 0x08 /* Private */,
       9,    1,   70,    2, 0x08 /* Private */,
      11,    1,   73,    2, 0x08 /* Private */,
      14,    1,   76,    2, 0x08 /* Private */,
      15,    0,   79,    2, 0x08 /* Private */,
      16,    0,   80,    2, 0x08 /* Private */,
      17,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QModelIndex,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SheetDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SheetDialog *_t = static_cast<SheetDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sheetSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onStripConfigTableChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onDoubleStripChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onSheetTypeComChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onDialogButtonClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: _t->onSheetSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->onSheetInsert(); break;
        case 7: _t->onSheetRemove(); break;
        case 8: _t->onSheetEdit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SheetDialog::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SheetDialog::sheetSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SheetDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SheetDialog.data,
      qt_meta_data_SheetDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SheetDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SheetDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SheetDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SheetDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SheetDialog::sheetSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
