/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonGroupPrivate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonGroupPrivate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonGroupPrivate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__GroupDefaultLayout_t {
    QByteArrayData data[1];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__GroupDefaultLayout_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__GroupDefaultLayout_t qt_meta_stringdata_Qtitan__GroupDefaultLayout = {
    {
QT_MOC_LITERAL(0, 0, 26) // "Qtitan::GroupDefaultLayout"

    },
    "Qtitan::GroupDefaultLayout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__GroupDefaultLayout[] = {

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

void Qtitan::GroupDefaultLayout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::GroupDefaultLayout::staticMetaObject = {
    { &GroupLayout::staticMetaObject, qt_meta_stringdata_Qtitan__GroupDefaultLayout.data,
      qt_meta_data_Qtitan__GroupDefaultLayout,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::GroupDefaultLayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::GroupDefaultLayout::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__GroupDefaultLayout.stringdata0))
        return static_cast<void*>(const_cast< GroupDefaultLayout*>(this));
    return GroupLayout::qt_metacast(_clname);
}

int Qtitan::GroupDefaultLayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GroupLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonGroupPrivate_t {
    QByteArrayData data[7];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonGroupPrivate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonGroupPrivate_t qt_meta_stringdata_Qtitan__RibbonGroupPrivate = {
    {
QT_MOC_LITERAL(0, 0, 26), // "Qtitan::RibbonGroupPrivate"
QT_MOC_LITERAL(1, 27, 21), // "pressLeftScrollButton"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 22), // "pressRightScrollButton"
QT_MOC_LITERAL(4, 73, 13), // "hidePopupMode"
QT_MOC_LITERAL(5, 87, 8), // "QAction*"
QT_MOC_LITERAL(6, 96, 6) // "action"

    },
    "Qtitan::RibbonGroupPrivate\0"
    "pressLeftScrollButton\0\0pressRightScrollButton\0"
    "hidePopupMode\0QAction*\0action"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonGroupPrivate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    1,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void Qtitan::RibbonGroupPrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonGroupPrivate *_t = static_cast<RibbonGroupPrivate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pressLeftScrollButton(); break;
        case 1: _t->pressRightScrollButton(); break;
        case 2: _t->hidePopupMode((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Qtitan::RibbonGroupPrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonGroupPrivate.data,
      qt_meta_data_Qtitan__RibbonGroupPrivate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonGroupPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonGroupPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonGroupPrivate.stringdata0))
        return static_cast<void*>(const_cast< RibbonGroupPrivate*>(this));
    return QObject::qt_metacast(_clname);
}

int Qtitan::RibbonGroupPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
