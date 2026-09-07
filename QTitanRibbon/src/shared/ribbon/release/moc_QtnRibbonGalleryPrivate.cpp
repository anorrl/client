/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonGalleryPrivate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonGalleryPrivate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonGalleryPrivate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonGalleryPrivate_t {
    QByteArrayData data[8];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonGalleryPrivate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonGalleryPrivate_t qt_meta_stringdata_Qtitan__RibbonGalleryPrivate = {
    {
QT_MOC_LITERAL(0, 0, 28), // "Qtitan::RibbonGalleryPrivate"
QT_MOC_LITERAL(1, 29, 12), // "setScrollPos"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 10), // "nScrollPos"
QT_MOC_LITERAL(4, 54, 15), // "actionTriggered"
QT_MOC_LITERAL(5, 70, 6), // "action"
QT_MOC_LITERAL(6, 77, 15), // "pressedScrollUp"
QT_MOC_LITERAL(7, 93, 17) // "pressedScrollDown"

    },
    "Qtitan::RibbonGalleryPrivate\0setScrollPos\0"
    "\0nScrollPos\0actionTriggered\0action\0"
    "pressedScrollUp\0pressedScrollDown"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonGalleryPrivate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       6,    0,   40,    2, 0x0a /* Public */,
       7,    0,   41,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Qtitan::RibbonGalleryPrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonGalleryPrivate *_t = static_cast<RibbonGalleryPrivate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setScrollPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->actionTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->pressedScrollUp(); break;
        case 3: _t->pressedScrollDown(); break;
        default: ;
        }
    }
}

const QMetaObject Qtitan::RibbonGalleryPrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonGalleryPrivate.data,
      qt_meta_data_Qtitan__RibbonGalleryPrivate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonGalleryPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonGalleryPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonGalleryPrivate.stringdata0))
        return static_cast<void*>(const_cast< RibbonGalleryPrivate*>(this));
    return QObject::qt_metacast(_clname);
}

int Qtitan::RibbonGalleryPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
