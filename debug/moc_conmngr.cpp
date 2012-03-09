/****************************************************************************
** Meta object code from reading C++ file 'conmngr.h'
**
** Created: Fri Mar 9 17:25:51 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../conmngr.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conmngr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConnectionManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      32,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   18,   18,   18, 0x08,
      75,   18,   18,   18, 0x08,
      96,   18,   18,   18, 0x08,
     110,   18,   18,   18, 0x08,
     131,   18,   18,   18, 0x08,
     151,   18,   18,   18, 0x08,
     167,   18,   18,   18, 0x08,
     189,   18,   18,   18, 0x08,
     209,   18,   18,   18, 0x08,
     236,   18,   18,   18, 0x08,
     262,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConnectionManager[] = {
    "ConnectionManager\0\0NewMessage()\0"
    "NicknamesChanged()\0OnIncommingConnection()\0"
    "OnPendingDatagrams()\0OnReadTcpTo()\0"
    "OnReadTcpIncomming()\0OnReadTcpOutgoing()\0"
    "OnReadTcpFrom()\0OnTcpFromDisconnect()\0"
    "OnTcpToDisconnect()\0OnTcpIncommingDisconnect()\0"
    "OnTcpOutgoingDisconnect()\0"
    "OnHandshakeOutgoing()\0"
};

const QMetaObject ConnectionManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConnectionManager,
      qt_meta_data_ConnectionManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConnectionManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConnectionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConnectionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionManager))
        return static_cast<void*>(const_cast< ConnectionManager*>(this));
    return QObject::qt_metacast(_clname);
}

int ConnectionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: NewMessage(); break;
        case 1: NicknamesChanged(); break;
        case 2: OnIncommingConnection(); break;
        case 3: OnPendingDatagrams(); break;
        case 4: OnReadTcpTo(); break;
        case 5: OnReadTcpIncomming(); break;
        case 6: OnReadTcpOutgoing(); break;
        case 7: OnReadTcpFrom(); break;
        case 8: OnTcpFromDisconnect(); break;
        case 9: OnTcpToDisconnect(); break;
        case 10: OnTcpIncommingDisconnect(); break;
        case 11: OnTcpOutgoingDisconnect(); break;
        case 12: OnHandshakeOutgoing(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ConnectionManager::NewMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ConnectionManager::NicknamesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
