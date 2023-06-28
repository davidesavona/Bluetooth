/****************************************************************************
** Meta object code from reading C++ file 'concurrentbtle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../concurrentbtle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'concurrentbtle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConcurrentBtle_t {
    QByteArrayData data[10];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConcurrentBtle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConcurrentBtle_t qt_meta_stringdata_ConcurrentBtle = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ConcurrentBtle"
QT_MOC_LITERAL(1, 15, 11), // "startSearch"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 19), // "establishConnection"
QT_MOC_LITERAL(4, 48, 21), // "setupNotificationLeft"
QT_MOC_LITERAL(5, 70, 21), // "QLowEnergyController*"
QT_MOC_LITERAL(6, 92, 6), // "device"
QT_MOC_LITERAL(7, 99, 4), // "name"
QT_MOC_LITERAL(8, 104, 22), // "setupNotificationRight"
QT_MOC_LITERAL(9, 127, 23) // "setupNotificationCardio"

    },
    "ConcurrentBtle\0startSearch\0\0"
    "establishConnection\0setupNotificationLeft\0"
    "QLowEnergyController*\0device\0name\0"
    "setupNotificationRight\0setupNotificationCardio"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConcurrentBtle[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    2,   41,    2, 0x0a /* Public */,
       8,    2,   46,    2, 0x0a /* Public */,
       9,    2,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString,    6,    7,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString,    6,    7,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString,    6,    7,

       0        // eod
};

void ConcurrentBtle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConcurrentBtle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startSearch(); break;
        case 1: _t->establishConnection(); break;
        case 2: _t->setupNotificationLeft((*reinterpret_cast< QLowEnergyController*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->setupNotificationRight((*reinterpret_cast< QLowEnergyController*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->setupNotificationCardio((*reinterpret_cast< QLowEnergyController*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConcurrentBtle::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ConcurrentBtle.data,
    qt_meta_data_ConcurrentBtle,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConcurrentBtle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConcurrentBtle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConcurrentBtle.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ConcurrentBtle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
