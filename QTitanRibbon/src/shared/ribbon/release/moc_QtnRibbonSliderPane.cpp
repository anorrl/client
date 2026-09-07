/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonSliderPane.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonSliderPane.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonSliderPane.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonSliderPane_t {
    QByteArrayData data[11];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonSliderPane_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonSliderPane_t qt_meta_stringdata_Qtitan__RibbonSliderPane = {
    {
QT_MOC_LITERAL(0, 0, 24), // "Qtitan::RibbonSliderPane"
QT_MOC_LITERAL(1, 25, 12), // "valueChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 5), // "value"
QT_MOC_LITERAL(4, 45, 11), // "sliderMoved"
QT_MOC_LITERAL(5, 57, 8), // "position"
QT_MOC_LITERAL(6, 66, 8), // "setValue"
QT_MOC_LITERAL(7, 75, 9), // "increment"
QT_MOC_LITERAL(8, 85, 9), // "decrement"
QT_MOC_LITERAL(9, 95, 10), // "singleStep"
QT_MOC_LITERAL(10, 106, 14) // "sliderPosition"

    },
    "Qtitan::RibbonSliderPane\0valueChanged\0"
    "\0value\0sliderMoved\0position\0setValue\0"
    "increment\0decrement\0singleStep\0"
    "sliderPosition"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonSliderPane[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   45,    2, 0x0a /* Public */,
       7,    0,   48,    2, 0x09 /* Protected */,
       8,    0,   49,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       9, QMetaType::Int, 0x00095103,
       3, QMetaType::Int, 0x00595103,
      10, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,
       0,
       1,

       0        // eod
};

void Qtitan::RibbonSliderPane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonSliderPane *_t = static_cast<RibbonSliderPane *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->increment(); break;
        case 4: _t->decrement(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RibbonSliderPane::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonSliderPane::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (RibbonSliderPane::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonSliderPane::sliderMoved)) {
                *result = 1;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        RibbonSliderPane *_t = static_cast<RibbonSliderPane *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->singleStep(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->value(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->sliderPosition(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        RibbonSliderPane *_t = static_cast<RibbonSliderPane *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSingleStep(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setValue(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setSliderPosition(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Qtitan::RibbonSliderPane::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonSliderPane.data,
      qt_meta_data_Qtitan__RibbonSliderPane,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonSliderPane::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonSliderPane::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonSliderPane.stringdata0))
        return static_cast<void*>(const_cast< RibbonSliderPane*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qtitan::RibbonSliderPane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Qtitan::RibbonSliderPane::valueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Qtitan::RibbonSliderPane::sliderMoved(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
