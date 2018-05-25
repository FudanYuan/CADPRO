/****************************************************************************
** Meta object code from reading C++ file 'sketchconfiguredialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../configure/sketchconfiguredialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sketchconfiguredialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CustomTabWidget_t {
    QByteArrayData data[10];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CustomTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CustomTabWidget_t qt_meta_stringdata_CustomTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CustomTabWidget"
QT_MOC_LITERAL(1, 16, 10), // "tabChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "onColorChanged"
QT_MOC_LITERAL(4, 43, 3), // "key"
QT_MOC_LITERAL(5, 47, 5), // "color"
QT_MOC_LITERAL(6, 53, 13), // "onTextChanged"
QT_MOC_LITERAL(7, 67, 5), // "value"
QT_MOC_LITERAL(8, 73, 17), // "onComboBoxChanged"
QT_MOC_LITERAL(9, 91, 14) // "onCheckChanged"

    },
    "CustomTabWidget\0tabChanged\0\0onColorChanged\0"
    "key\0color\0onTextChanged\0value\0"
    "onComboBoxChanged\0onCheckChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   44,    2, 0x0a /* Public */,
       6,    2,   49,    2, 0x0a /* Public */,
       8,    2,   54,    2, 0x0a /* Public */,
       9,    2,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QColor,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    4,    7,

       0        // eod
};

void CustomTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CustomTabWidget *_t = static_cast<CustomTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tabChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 1: _t->onColorChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 2: _t->onTextChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->onComboBoxChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->onCheckChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CustomTabWidget::*_t)(QString , QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CustomTabWidget::tabChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CustomTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CustomTabWidget.data,
      qt_meta_data_CustomTabWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CustomTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CustomTabWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CustomTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CustomTabWidget::tabChanged(QString _t1, QVariant _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_EntityStyleTab_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EntityStyleTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EntityStyleTab_t qt_meta_stringdata_EntityStyleTab = {
    {
QT_MOC_LITERAL(0, 0, 14) // "EntityStyleTab"

    },
    "EntityStyleTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EntityStyleTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void EntityStyleTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject EntityStyleTab::staticMetaObject = {
    { &CustomTabWidget::staticMetaObject, qt_meta_stringdata_EntityStyleTab.data,
      qt_meta_data_EntityStyleTab,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EntityStyleTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EntityStyleTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EntityStyleTab.stringdata0))
        return static_cast<void*>(this);
    return CustomTabWidget::qt_metacast(_clname);
}

int EntityStyleTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CustomTabWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_AxesGridTab_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AxesGridTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AxesGridTab_t qt_meta_stringdata_AxesGridTab = {
    {
QT_MOC_LITERAL(0, 0, 11) // "AxesGridTab"

    },
    "AxesGridTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AxesGridTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AxesGridTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AxesGridTab::staticMetaObject = {
    { &CustomTabWidget::staticMetaObject, qt_meta_stringdata_AxesGridTab.data,
      qt_meta_data_AxesGridTab,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AxesGridTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AxesGridTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AxesGridTab.stringdata0))
        return static_cast<void*>(this);
    return CustomTabWidget::qt_metacast(_clname);
}

int AxesGridTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CustomTabWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_OffsetTab_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OffsetTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OffsetTab_t qt_meta_stringdata_OffsetTab = {
    {
QT_MOC_LITERAL(0, 0, 9) // "OffsetTab"

    },
    "OffsetTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OffsetTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void OffsetTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject OffsetTab::staticMetaObject = {
    { &CustomTabWidget::staticMetaObject, qt_meta_stringdata_OffsetTab.data,
      qt_meta_data_OffsetTab,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OffsetTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OffsetTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OffsetTab.stringdata0))
        return static_cast<void*>(this);
    return CustomTabWidget::qt_metacast(_clname);
}

int OffsetTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CustomTabWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_LanguageTab_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LanguageTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LanguageTab_t qt_meta_stringdata_LanguageTab = {
    {
QT_MOC_LITERAL(0, 0, 11) // "LanguageTab"

    },
    "LanguageTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LanguageTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void LanguageTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject LanguageTab::staticMetaObject = {
    { &CustomTabWidget::staticMetaObject, qt_meta_stringdata_LanguageTab.data,
      qt_meta_data_LanguageTab,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LanguageTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LanguageTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LanguageTab.stringdata0))
        return static_cast<void*>(this);
    return CustomTabWidget::qt_metacast(_clname);
}

int LanguageTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CustomTabWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SketchConfigureDialog_t {
    QByteArrayData data[9];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SketchConfigureDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SketchConfigureDialog_t qt_meta_stringdata_SketchConfigureDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "SketchConfigureDialog"
QT_MOC_LITERAL(1, 22, 7), // "changed"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 32), // "QList<SketchConfigure::KeyValue>"
QT_MOC_LITERAL(4, 64, 6), // "accept"
QT_MOC_LITERAL(5, 71, 6), // "reject"
QT_MOC_LITERAL(6, 78, 9), // "onChanged"
QT_MOC_LITERAL(7, 88, 3), // "key"
QT_MOC_LITERAL(8, 92, 5) // "value"

    },
    "SketchConfigureDialog\0changed\0\0"
    "QList<SketchConfigure::KeyValue>\0"
    "accept\0reject\0onChanged\0key\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SketchConfigureDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       6,    2,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    7,    8,

       0        // eod
};

void SketchConfigureDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SketchConfigureDialog *_t = static_cast<SketchConfigureDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< QList<SketchConfigure::KeyValue>(*)>(_a[1]))); break;
        case 1: _t->accept(); break;
        case 2: _t->reject(); break;
        case 3: _t->onChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SketchConfigureDialog::*_t)(QList<SketchConfigure::KeyValue> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SketchConfigureDialog::changed)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SketchConfigureDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SketchConfigureDialog.data,
      qt_meta_data_SketchConfigureDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SketchConfigureDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SketchConfigureDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SketchConfigureDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SketchConfigureDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SketchConfigureDialog::changed(QList<SketchConfigure::KeyValue> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
