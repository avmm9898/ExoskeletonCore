/****************************************************************************
** Meta object code from reading C++ file 'chserialport.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chserialport.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chserialport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CHSerialPort_t {
    QByteArrayData data[16];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CHSerialPort_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CHSerialPort_t qt_meta_stringdata_CHSerialPort = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CHSerialPort"
QT_MOC_LITERAL(1, 13, 10), // "sigSendIMU"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 23), // "receive_imusol_packet_t"
QT_MOC_LITERAL(4, 49, 13), // "errorOpenPort"
QT_MOC_LITERAL(5, 63, 13), // "sigPortOpened"
QT_MOC_LITERAL(6, 77, 13), // "sigPortClosed"
QT_MOC_LITERAL(7, 91, 21), // "sigCloseThreadAndPort"
QT_MOC_LITERAL(8, 113, 9), // "closePort"
QT_MOC_LITERAL(9, 123, 13), // "linkCHdevices"
QT_MOC_LITERAL(10, 137, 11), // "quitmThread"
QT_MOC_LITERAL(11, 149, 17), // "on_thread_started"
QT_MOC_LITERAL(12, 167, 17), // "on_thread_stopped"
QT_MOC_LITERAL(13, 185, 17), // "initThreadReading"
QT_MOC_LITERAL(14, 203, 18), // "closeThreadAndPort"
QT_MOC_LITERAL(15, 222, 10) // "handleData"

    },
    "CHSerialPort\0sigSendIMU\0\0"
    "receive_imusol_packet_t\0errorOpenPort\0"
    "sigPortOpened\0sigPortClosed\0"
    "sigCloseThreadAndPort\0closePort\0"
    "linkCHdevices\0quitmThread\0on_thread_started\0"
    "on_thread_stopped\0initThreadReading\0"
    "closeThreadAndPort\0handleData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CHSerialPort[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    0,   82,    2, 0x06 /* Public */,
       5,    0,   83,    2, 0x06 /* Public */,
       6,    0,   84,    2, 0x06 /* Public */,
       7,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   86,    2, 0x0a /* Public */,
       9,    2,   87,    2, 0x0a /* Public */,
      10,    0,   92,    2, 0x0a /* Public */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CHSerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CHSerialPort *_t = static_cast<CHSerialPort *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSendIMU((*reinterpret_cast< receive_imusol_packet_t(*)>(_a[1]))); break;
        case 1: _t->errorOpenPort(); break;
        case 2: _t->sigPortOpened(); break;
        case 3: _t->sigPortClosed(); break;
        case 4: _t->sigCloseThreadAndPort(); break;
        case 5: _t->closePort(); break;
        case 6: _t->linkCHdevices((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->quitmThread(); break;
        case 8: _t->on_thread_started(); break;
        case 9: _t->on_thread_stopped(); break;
        case 10: _t->initThreadReading(); break;
        case 11: _t->closeThreadAndPort(); break;
        case 12: _t->handleData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CHSerialPort::*)(receive_imusol_packet_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHSerialPort::sigSendIMU)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CHSerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHSerialPort::errorOpenPort)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CHSerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHSerialPort::sigPortOpened)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CHSerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHSerialPort::sigPortClosed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CHSerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHSerialPort::sigCloseThreadAndPort)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CHSerialPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHSerialPort.data,
      qt_meta_data_CHSerialPort,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CHSerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CHSerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CHSerialPort.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CHSerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CHSerialPort::sigSendIMU(receive_imusol_packet_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CHSerialPort::errorOpenPort()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CHSerialPort::sigPortOpened()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CHSerialPort::sigPortClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CHSerialPort::sigCloseThreadAndPort()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
