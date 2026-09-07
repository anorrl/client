/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonBackstageViewPrivate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonBackstageViewPrivate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonBackstageViewPrivate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton_t {
    QByteArrayData data[1];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton_t qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton = {
    {
QT_MOC_LITERAL(0, 0, 34) // "Qtitan::RibbonBackstageCloseB..."

    },
    "Qtitan::RibbonBackstageCloseButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonBackstageCloseButton[] = {

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

void Qtitan::RibbonBackstageCloseButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonBackstageCloseButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton.data,
      qt_meta_data_Qtitan__RibbonBackstageCloseButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonBackstageCloseButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonBackstageCloseButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonBackstageCloseButton.stringdata0))
        return static_cast<void*>(const_cast< RibbonBackstageCloseButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int Qtitan::RibbonBackstageCloseButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate_t {
    QByteArrayData data[3];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate_t qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate = {
    {
QT_MOC_LITERAL(0, 0, 34), // "Qtitan::RibbonBackstageViewPr..."
QT_MOC_LITERAL(1, 35, 15), // "trackMouseEvent"
QT_MOC_LITERAL(2, 51, 0) // ""

    },
    "Qtitan::RibbonBackstageViewPrivate\0"
    "trackMouseEvent\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonBackstageViewPrivate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Qtitan::RibbonBackstageViewPrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonBackstageViewPrivate *_t = static_cast<RibbonBackstageViewPrivate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->trackMouseEvent(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonBackstageViewPrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate.data,
      qt_meta_data_Qtitan__RibbonBackstageViewPrivate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonBackstageViewPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonBackstageViewPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonBackstageViewPrivate.stringdata0))
        return static_cast<void*>(const_cast< RibbonBackstageViewPrivate*>(this));
    return QObject::qt_metacast(_clname);
}

int Qtitan::RibbonBackstageViewPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
