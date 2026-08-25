/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonBar_t {
    QByteArrayData data[25];
    char stringdata0[322];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonBar_t qt_meta_stringdata_Qtitan__RibbonBar = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Qtitan::RibbonBar"
QT_MOC_LITERAL(1, 18, 19), // "minimizationChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 9), // "minimized"
QT_MOC_LITERAL(4, 49, 18), // "currentPageChanged"
QT_MOC_LITERAL(5, 68, 5), // "index"
QT_MOC_LITERAL(6, 74, 11), // "RibbonPage*"
QT_MOC_LITERAL(7, 86, 4), // "page"
QT_MOC_LITERAL(8, 91, 13), // "keyTipsShowed"
QT_MOC_LITERAL(9, 105, 6), // "showed"
QT_MOC_LITERAL(10, 112, 21), // "showRibbonContextMenu"
QT_MOC_LITERAL(11, 134, 6), // "QMenu*"
QT_MOC_LITERAL(12, 141, 4), // "menu"
QT_MOC_LITERAL(13, 146, 5), // "bool&"
QT_MOC_LITERAL(14, 152, 8), // "showMenu"
QT_MOC_LITERAL(15, 161, 15), // "ribbonCustomize"
QT_MOC_LITERAL(16, 177, 7), // "userDlg"
QT_MOC_LITERAL(17, 185, 14), // "setCurrentPage"
QT_MOC_LITERAL(18, 200, 22), // "quickAccessBarPosition"
QT_MOC_LITERAL(19, 223, 22), // "QuickAccessBarPosition"
QT_MOC_LITERAL(20, 246, 16), // "currentIndexPage"
QT_MOC_LITERAL(21, 263, 17), // "frameThemeEnabled"
QT_MOC_LITERAL(22, 281, 9), // "maximized"
QT_MOC_LITERAL(23, 291, 13), // "QATopPosition"
QT_MOC_LITERAL(24, 305, 16) // "QABottomPosition"

    },
    "Qtitan::RibbonBar\0minimizationChanged\0"
    "\0minimized\0currentPageChanged\0index\0"
    "RibbonPage*\0page\0keyTipsShowed\0showed\0"
    "showRibbonContextMenu\0QMenu*\0menu\0"
    "bool&\0showMenu\0ribbonCustomize\0userDlg\0"
    "setCurrentPage\0quickAccessBarPosition\0"
    "QuickAccessBarPosition\0currentIndexPage\0"
    "frameThemeEnabled\0maximized\0QATopPosition\0"
    "QABottomPosition"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       5,   72, // properties
       1,   92, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       4,    1,   55,    2, 0x06 /* Public */,
       8,    1,   58,    2, 0x06 /* Public */,
      10,    2,   61,    2, 0x06 /* Public */,
      15,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13,   12,   14,
    QMetaType::Void, 0x80000000 | 13,   16,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,

 // properties: name, type, flags
      18, 0x80000000 | 19, 0x0009510b,
      20, QMetaType::Int, 0x00495003,
      21, QMetaType::Bool, 0x00095103,
       3, QMetaType::Bool, 0x00495103,
      22, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       0,
       0,
       0,

 // enums: name, flags, count, data
      19, 0x0,    2,   96,

 // enum data: key, value
      23, uint(Qtitan::RibbonBar::QATopPosition),
      24, uint(Qtitan::RibbonBar::QABottomPosition),

       0        // eod
};

void Qtitan::RibbonBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonBar *_t = static_cast<RibbonBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->minimizationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->currentPageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->currentPageChanged((*reinterpret_cast< RibbonPage*(*)>(_a[1]))); break;
        case 3: _t->keyTipsShowed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->showRibbonContextMenu((*reinterpret_cast< QMenu*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->ribbonCustomize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setCurrentPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RibbonBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::minimizationChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RibbonBar::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::currentPageChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RibbonBar::*_t)(RibbonPage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::currentPageChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RibbonBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::keyTipsShowed)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RibbonBar::*_t)(QMenu * , bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::showRibbonContextMenu)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RibbonBar::*_t)(bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonBar::ribbonCustomize)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        RibbonBar *_t = static_cast<RibbonBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QuickAccessBarPosition*>(_v) = _t->quickAccessBarPosition(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->currentIndexPage(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isFrameThemeEnabled(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isMinimized(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isMaximized(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        RibbonBar *_t = static_cast<RibbonBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setQuickAccessBarPosition(*reinterpret_cast< QuickAccessBarPosition*>(_v)); break;
        case 1: _t->setCurrentPage(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setFrameThemeEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setMinimized(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setMaximized(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Qtitan::RibbonBar::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonBar.data,
      qt_meta_data_Qtitan__RibbonBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonBar.stringdata0))
        return static_cast<void*>(const_cast< RibbonBar*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int Qtitan::RibbonBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Qtitan::RibbonBar::minimizationChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Qtitan::RibbonBar::currentPageChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Qtitan::RibbonBar::currentPageChanged(RibbonPage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Qtitan::RibbonBar::keyTipsShowed(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Qtitan::RibbonBar::showRibbonContextMenu(QMenu * _t1, bool & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Qtitan::RibbonBar::ribbonCustomize(bool & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
