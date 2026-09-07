/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonGallery.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonGallery.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonGallery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonGalleryGroup_t {
    QByteArrayData data[1];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonGalleryGroup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonGalleryGroup_t qt_meta_stringdata_Qtitan__RibbonGalleryGroup = {
    {
QT_MOC_LITERAL(0, 0, 26) // "Qtitan::RibbonGalleryGroup"

    },
    "Qtitan::RibbonGalleryGroup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonGalleryGroup[] = {

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

void Qtitan::RibbonGalleryGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonGalleryGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonGalleryGroup.data,
      qt_meta_data_Qtitan__RibbonGalleryGroup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonGalleryGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonGalleryGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonGalleryGroup.stringdata0))
        return static_cast<void*>(const_cast< RibbonGalleryGroup*>(this));
    return QObject::qt_metacast(_clname);
}

int Qtitan::RibbonGalleryGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonGallery_t {
    QByteArrayData data[17];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonGallery_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonGallery_t qt_meta_stringdata_Qtitan__RibbonGallery = {
    {
QT_MOC_LITERAL(0, 0, 21), // "Qtitan::RibbonGallery"
QT_MOC_LITERAL(1, 22, 11), // "itemPressed"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 18), // "RibbonGalleryItem*"
QT_MOC_LITERAL(4, 54, 4), // "item"
QT_MOC_LITERAL(5, 59, 11), // "itemClicked"
QT_MOC_LITERAL(6, 71, 12), // "itemClicking"
QT_MOC_LITERAL(7, 84, 5), // "bool&"
QT_MOC_LITERAL(8, 90, 7), // "handled"
QT_MOC_LITERAL(9, 98, 18), // "currentItemChanged"
QT_MOC_LITERAL(10, 117, 7), // "current"
QT_MOC_LITERAL(11, 125, 8), // "previous"
QT_MOC_LITERAL(12, 134, 20), // "itemSelectionChanged"
QT_MOC_LITERAL(13, 155, 15), // "isBorderVisible"
QT_MOC_LITERAL(14, 171, 15), // "scrollBarPolicy"
QT_MOC_LITERAL(15, 187, 19), // "Qt::ScrollBarPolicy"
QT_MOC_LITERAL(16, 207, 11) // "isResizable"

    },
    "Qtitan::RibbonGallery\0itemPressed\0\0"
    "RibbonGalleryItem*\0item\0itemClicked\0"
    "itemClicking\0bool&\0handled\0"
    "currentItemChanged\0current\0previous\0"
    "itemSelectionChanged\0isBorderVisible\0"
    "scrollBarPolicy\0Qt::ScrollBarPolicy\0"
    "isResizable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonGallery[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       6,    2,   45,    2, 0x06 /* Public */,
       9,    2,   50,    2, 0x06 /* Public */,
      12,    0,   55,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 7,    4,    8,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,   10,   11,
    QMetaType::Void,

 // properties: name, type, flags
      13, QMetaType::Bool, 0x00095003,
      14, 0x80000000 | 15, 0x0009510b,
      16, QMetaType::Bool, 0x00095001,

       0        // eod
};

void Qtitan::RibbonGallery::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonGallery *_t = static_cast<RibbonGallery *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->itemPressed((*reinterpret_cast< RibbonGalleryItem*(*)>(_a[1]))); break;
        case 1: _t->itemClicked((*reinterpret_cast< RibbonGalleryItem*(*)>(_a[1]))); break;
        case 2: _t->itemClicking((*reinterpret_cast< RibbonGalleryItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->currentItemChanged((*reinterpret_cast< RibbonGalleryItem*(*)>(_a[1])),(*reinterpret_cast< RibbonGalleryItem*(*)>(_a[2]))); break;
        case 4: _t->itemSelectionChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RibbonGallery::*_t)(RibbonGalleryItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGallery::itemPressed)) {
                *result = 0;
            }
        }
        {
            typedef void (RibbonGallery::*_t)(RibbonGalleryItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGallery::itemClicked)) {
                *result = 1;
            }
        }
        {
            typedef void (RibbonGallery::*_t)(RibbonGalleryItem * , bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGallery::itemClicking)) {
                *result = 2;
            }
        }
        {
            typedef void (RibbonGallery::*_t)(RibbonGalleryItem * , RibbonGalleryItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGallery::currentItemChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (RibbonGallery::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGallery::itemSelectionChanged)) {
                *result = 4;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        RibbonGallery *_t = static_cast<RibbonGallery *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isBorderVisible(); break;
        case 1: *reinterpret_cast< Qt::ScrollBarPolicy*>(_v) = _t->scrollBarPolicy(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isResizable(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        RibbonGallery *_t = static_cast<RibbonGallery *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBorderVisible(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setScrollBarPolicy(*reinterpret_cast< Qt::ScrollBarPolicy*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Qtitan::RibbonGallery::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonGallery.data,
      qt_meta_data_Qtitan__RibbonGallery,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonGallery::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonGallery::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonGallery.stringdata0))
        return static_cast<void*>(const_cast< RibbonGallery*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qtitan::RibbonGallery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Qtitan::RibbonGallery::itemPressed(RibbonGalleryItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Qtitan::RibbonGallery::itemClicked(RibbonGalleryItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Qtitan::RibbonGallery::itemClicking(RibbonGalleryItem * _t1, bool & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Qtitan::RibbonGallery::currentItemChanged(RibbonGalleryItem * _t1, RibbonGalleryItem * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Qtitan::RibbonGallery::itemSelectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
