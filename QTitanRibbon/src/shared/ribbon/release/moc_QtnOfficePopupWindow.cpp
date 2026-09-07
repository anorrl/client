/****************************************************************************
** Meta object code from reading C++ file 'QtnOfficePopupWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ribbon/QtnOfficePopupWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtnOfficePopupWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Qtitan__OfficePopupWindow_t {
    QByteArrayData data[19];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtitan__OfficePopupWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtitan__OfficePopupWindow_t qt_meta_stringdata_Qtitan__OfficePopupWindow = {
    {
QT_MOC_LITERAL(0, 0, 25), // "Qtitan::OfficePopupWindow"
QT_MOC_LITERAL(1, 26, 11), // "aboutToShow"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "aboutToHide"
QT_MOC_LITERAL(4, 51, 10), // "closePopup"
QT_MOC_LITERAL(5, 62, 14), // "showDelayTimer"
QT_MOC_LITERAL(6, 77, 15), // "collapsingTimer"
QT_MOC_LITERAL(7, 93, 14), // "expandingTimer"
QT_MOC_LITERAL(8, 108, 9), // "animation"
QT_MOC_LITERAL(9, 118, 14), // "PopupAnimation"
QT_MOC_LITERAL(10, 133, 12), // "transparency"
QT_MOC_LITERAL(11, 146, 11), // "displayTime"
QT_MOC_LITERAL(12, 158, 14), // "animationSpeed"
QT_MOC_LITERAL(13, 173, 9), // "titleIcon"
QT_MOC_LITERAL(14, 183, 9), // "titleText"
QT_MOC_LITERAL(15, 193, 8), // "bodyText"
QT_MOC_LITERAL(16, 202, 23), // "titleCloseButtonVisible"
QT_MOC_LITERAL(17, 226, 8), // "location"
QT_MOC_LITERAL(18, 235, 13) // "PopupLocation"

    },
    "Qtitan::OfficePopupWindow\0aboutToShow\0"
    "\0aboutToHide\0closePopup\0showDelayTimer\0"
    "collapsingTimer\0expandingTimer\0animation\0"
    "PopupAnimation\0transparency\0displayTime\0"
    "animationSpeed\0titleIcon\0titleText\0"
    "bodyText\0titleCloseButtonVisible\0"
    "location\0PopupLocation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtitan__OfficePopupWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       9,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x09 /* Protected */,
       6,    0,   48,    2, 0x09 /* Protected */,
       7,    0,   49,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       8, 0x80000000 | 9, 0x0009510b,
      10, QMetaType::QReal, 0x00095103,
      11, QMetaType::Int, 0x00095103,
      12, QMetaType::Int, 0x00095103,
      13, QMetaType::QIcon, 0x00095103,
      14, QMetaType::QString, 0x00095103,
      15, QMetaType::QString, 0x00095103,
      16, QMetaType::Bool, 0x00095103,
      17, 0x80000000 | 18, 0x0009510b,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

void Qtitan::OfficePopupWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OfficePopupWindow *_t = static_cast<OfficePopupWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->aboutToShow(); break;
        case 1: _t->aboutToHide(); break;
        case 2: _t->closePopup(); break;
        case 3: _t->showDelayTimer(); break;
        case 4: _t->collapsingTimer(); break;
        case 5: _t->expandingTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (OfficePopupWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OfficePopupWindow::aboutToShow)) {
                *result = 0;
            }
        }
        {
            typedef void (OfficePopupWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OfficePopupWindow::aboutToHide)) {
                *result = 1;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        OfficePopupWindow *_t = static_cast<OfficePopupWindow *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< PopupAnimation*>(_v) = _t->animation(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->transparency(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->displayTime(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->animationSpeed(); break;
        case 4: *reinterpret_cast< QIcon*>(_v) = _t->titleIcon(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->titleText(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->bodyText(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->isTitleCloseButtonVisible(); break;
        case 8: *reinterpret_cast< PopupLocation*>(_v) = _t->location(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        OfficePopupWindow *_t = static_cast<OfficePopupWindow *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setAnimation(*reinterpret_cast< PopupAnimation*>(_v)); break;
        case 1: _t->setTransparency(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setDisplayTime(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setAnimationSpeed(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setTitleIcon(*reinterpret_cast< QIcon*>(_v)); break;
        case 5: _t->setTitleText(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setBodyText(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setTitleCloseButtonVisible(*reinterpret_cast< bool*>(_v)); break;
        case 8: _t->setLocation(*reinterpret_cast< PopupLocation*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject Qtitan::OfficePopupWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qtitan__OfficePopupWindow.data,
      qt_meta_data_Qtitan__OfficePopupWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Qtitan::OfficePopupWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtitan::OfficePopupWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Qtitan__OfficePopupWindow.stringdata0))
        return static_cast<void*>(const_cast< OfficePopupWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qtitan::OfficePopupWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Qtitan::OfficePopupWindow::aboutToShow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Qtitan::OfficePopupWindow::aboutToHide()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
