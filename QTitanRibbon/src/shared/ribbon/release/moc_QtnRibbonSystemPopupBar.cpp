/****************************************************************************
** Meta object code from reading C++ file 'QtnRibbonSystemPopupBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnRibbonSystemPopupBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnRibbonSystemPopupBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__RibbonSystemButton_t {
    QByteArrayData data[3];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonSystemButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonSystemButton_t qt_meta_stringdata_Qtitan__RibbonSystemButton = {
    {
QT_MOC_LITERAL(0, 0, 26), // "Qtitan::RibbonSystemButton"
QT_MOC_LITERAL(1, 27, 15), // "toolButtonStyle"
QT_MOC_LITERAL(2, 43, 19) // "Qt::ToolButtonStyle"

    },
    "Qtitan::RibbonSystemButton\0toolButtonStyle\0"
    "Qt::ToolButtonStyle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonSystemButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, 0x80000000 | 2, 0x0009510b,

       0        // eod
};

void Qtitan::RibbonSystemButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        RibbonSystemButton *_t = static_cast<RibbonSystemButton *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::ToolButtonStyle*>(_v) = _t->toolButtonStyle(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        RibbonSystemButton *_t = static_cast<RibbonSystemButton *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setToolButtonStyle(*reinterpret_cast< Qt::ToolButtonStyle*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonSystemButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonSystemButton.data,
      qt_meta_data_Qtitan__RibbonSystemButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonSystemButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonSystemButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonSystemButton.stringdata0))
        return static_cast<void*>(const_cast< RibbonSystemButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int Qtitan::RibbonSystemButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonSystemPopupBar_t {
    QByteArrayData data[1];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonSystemPopupBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonSystemPopupBar_t qt_meta_stringdata_Qtitan__RibbonSystemPopupBar = {
    {
QT_MOC_LITERAL(0, 0, 28) // "Qtitan::RibbonSystemPopupBar"

    },
    "Qtitan::RibbonSystemPopupBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonSystemPopupBar[] = {

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

void Qtitan::RibbonSystemPopupBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonSystemPopupBar::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonSystemPopupBar.data,
      qt_meta_data_Qtitan__RibbonSystemPopupBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonSystemPopupBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonSystemPopupBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonSystemPopupBar.stringdata0))
        return static_cast<void*>(const_cast< RibbonSystemPopupBar*>(this));
    return QMenu::qt_metacast(_clname);
}

int Qtitan::RibbonSystemPopupBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption_t {
    QByteArrayData data[1];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption_t qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption = {
    {
QT_MOC_LITERAL(0, 0, 40) // "Qtitan::RibbonPageSystemPopup..."

    },
    "Qtitan::RibbonPageSystemPopupListCaption"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonPageSystemPopupListCaption[] = {

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

void Qtitan::RibbonPageSystemPopupListCaption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonPageSystemPopupListCaption::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption.data,
      qt_meta_data_Qtitan__RibbonPageSystemPopupListCaption,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonPageSystemPopupListCaption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonPageSystemPopupListCaption::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonPageSystemPopupListCaption.stringdata0))
        return static_cast<void*>(const_cast< RibbonPageSystemPopupListCaption*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qtitan::RibbonPageSystemPopupListCaption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList_t {
    QByteArrayData data[6];
    char stringdata0[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList_t qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList = {
    {
QT_MOC_LITERAL(0, 0, 38), // "Qtitan::RibbonPageSystemRecen..."
QT_MOC_LITERAL(1, 39, 14), // "openRecentFile"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 4), // "file"
QT_MOC_LITERAL(4, 60, 23), // "updateRecentFileActions"
QT_MOC_LITERAL(5, 84, 5) // "files"

    },
    "Qtitan::RibbonPageSystemRecentFileList\0"
    "openRecentFile\0\0file\0updateRecentFileActions\0"
    "files"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonPageSystemRecentFileList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    5,

       0        // eod
};

void Qtitan::RibbonPageSystemRecentFileList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RibbonPageSystemRecentFileList *_t = static_cast<RibbonPageSystemRecentFileList *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openRecentFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->updateRecentFileActions((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RibbonPageSystemRecentFileList::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RibbonPageSystemRecentFileList::openRecentFile)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Qtitan::RibbonPageSystemRecentFileList::staticMetaObject = {
    { &RibbonPageSystemPopupListCaption::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList.data,
      qt_meta_data_Qtitan__RibbonPageSystemRecentFileList,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonPageSystemRecentFileList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonPageSystemRecentFileList::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonPageSystemRecentFileList.stringdata0))
        return static_cast<void*>(const_cast< RibbonPageSystemRecentFileList*>(this));
    return RibbonPageSystemPopupListCaption::qt_metacast(_clname);
}

int Qtitan::RibbonPageSystemRecentFileList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RibbonPageSystemPopupListCaption::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Qtitan::RibbonPageSystemRecentFileList::openRecentFile(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Qtitan__RibbonPageSystemPopup_t {
    QByteArrayData data[1];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__RibbonPageSystemPopup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__RibbonPageSystemPopup_t qt_meta_stringdata_Qtitan__RibbonPageSystemPopup = {
    {
QT_MOC_LITERAL(0, 0, 29) // "Qtitan::RibbonPageSystemPopup"

    },
    "Qtitan::RibbonPageSystemPopup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__RibbonPageSystemPopup[] = {

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

void Qtitan::RibbonPageSystemPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::RibbonPageSystemPopup::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_Qtitan__RibbonPageSystemPopup.data,
      qt_meta_data_Qtitan__RibbonPageSystemPopup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::RibbonPageSystemPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::RibbonPageSystemPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__RibbonPageSystemPopup.stringdata0))
        return static_cast<void*>(const_cast< RibbonPageSystemPopup*>(this));
    return QMenu::qt_metacast(_clname);
}

int Qtitan::RibbonPageSystemPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
