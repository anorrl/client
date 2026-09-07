/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonGroup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonGroup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonGroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonGroup_t {
    QByteArrayData data[15];
    char stringdata0[184];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonGroup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonGroup_t qt_meta_stringdata_Qtitan__RibbonGroup = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Qtitan::RibbonGroup"
QT_MOC_LITERAL(1, 20, 8), // "released"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 15), // "actionTriggered"
QT_MOC_LITERAL(4, 46, 8), // "QAction*"
QT_MOC_LITERAL(5, 55, 9), // "hidePopup"
QT_MOC_LITERAL(6, 65, 12), // "titleChanged"
QT_MOC_LITERAL(7, 78, 5), // "title"
QT_MOC_LITERAL(8, 84, 19), // "currentIndexChanged"
QT_MOC_LITERAL(9, 104, 5), // "index"
QT_MOC_LITERAL(10, 110, 18), // "isControlsGrouping"
QT_MOC_LITERAL(11, 129, 19), // "isControlsCentering"
QT_MOC_LITERAL(12, 149, 7), // "reduced"
QT_MOC_LITERAL(13, 157, 4), // "icon"
QT_MOC_LITERAL(14, 162, 21) // "isOptionButtonVisible"

    },
    "Qtitan::RibbonGroup\0released\0\0"
    "actionTriggered\0QAction*\0hidePopup\0"
    "titleChanged\0title\0currentIndexChanged\0"
    "index\0isControlsGrouping\0isControlsCentering\0"
    "reduced\0icon\0isOptionButtonVisible"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonGroup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       6,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       5,    0,   43,    2, 0x06 /* Public */,
       6,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   47,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,

 // properties: name, type, flags
      10, QMetaType::Bool, 0x00095003,
      11, QMetaType::Bool, 0x00095003,
      12, QMetaType::Bool, 0x00095001,
       7, QMetaType::QString, 0x00095103,
      13, QMetaType::QIcon, 0x00095103,
      14, QMetaType::Bool, 0x00094003,

       0        // eod
};

void Qtitan::RibbonGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonGroup *_t = static_cast<RibbonGroup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->released(); break;
        case 1: _t->actionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->hidePopup(); break;
        case 3: _t->titleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RibbonGroup::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGroup::released)) {
                *result = 0;
            }
        }
        {
            typedef void (RibbonGroup::*_t)(QAction * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGroup::actionTriggered)) {
                *result = 1;
            }
        }
        {
            typedef void (RibbonGroup::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGroup::hidePopup)) {
                *result = 2;
            }
        }
        {
            typedef void (RibbonGroup::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonGroup::titleChanged)) {
                *result = 3;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        RibbonGroup *_t = static_cast<RibbonGroup *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isControlsGrouping(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isControlsCentering(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isReduced(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->title(); break;
        case 4: *reinterpret_cast< QIcon*>(_v) = _t->icon(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->isOptionButtonVisible(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        RibbonGroup *_t = static_cast<RibbonGroup *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setControlsGrouping(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setControlsCentering(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setTitle(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setIcon(*reinterpret_cast< QIcon*>(_v)); break;
        case 5: _t->setOptionButtonVisible(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Qtitan::RibbonGroup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonGroup.data,
      qt_meta_data_Qtitan__RibbonGroup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonGroup.stringdata0))
        return static_cast<void*>(const_cast< RibbonGroup*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qtitan::RibbonGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Qtitan::RibbonGroup::released()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Qtitan::RibbonGroup::actionTriggered(QAction * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Qtitan::RibbonGroup::hidePopup()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Qtitan::RibbonGroup::titleChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
