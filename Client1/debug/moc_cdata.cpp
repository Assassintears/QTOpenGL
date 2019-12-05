/****************************************************************************
** Meta object code from reading C++ file 'cdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../cdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CData_t {
    QByteArrayData data[20];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CData_t qt_meta_stringdata_CData = {
    {
QT_MOC_LITERAL(0, 0, 5), // "CData"
QT_MOC_LITERAL(1, 6, 7), // "receive"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 14), // "QJsonDocument&"
QT_MOC_LITERAL(4, 30, 3), // "doc"
QT_MOC_LITERAL(5, 34, 7), // "hasData"
QT_MOC_LITERAL(6, 42, 18), // "QVector<QVector3D>"
QT_MOC_LITERAL(7, 61, 9), // "localData"
QT_MOC_LITERAL(8, 71, 13), // "QVector<uint>"
QT_MOC_LITERAL(9, 85, 3), // "ind"
QT_MOC_LITERAL(10, 89, 16), // "acceptConnection"
QT_MOC_LITERAL(11, 106, 13), // "onDataProcess"
QT_MOC_LITERAL(12, 120, 6), // "errors"
QT_MOC_LITERAL(13, 127, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(14, 156, 9), // "connected"
QT_MOC_LITERAL(15, 166, 14), // "onDisconnected"
QT_MOC_LITERAL(16, 181, 7), // "readBuf"
QT_MOC_LITERAL(17, 189, 17), // "receiveDataFromDB"
QT_MOC_LITERAL(18, 207, 24), // "QVector<QVector<float> >"
QT_MOC_LITERAL(19, 232, 2) // "db"

    },
    "CData\0receive\0\0QJsonDocument&\0doc\0"
    "hasData\0QVector<QVector3D>\0localData\0"
    "QVector<uint>\0ind\0acceptConnection\0"
    "onDataProcess\0errors\0QAbstractSocket::SocketError\0"
    "connected\0onDisconnected\0readBuf\0"
    "receiveDataFromDB\0QVector<QVector<float> >\0"
    "db"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    2,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   67,    2, 0x0a /* Public */,
      11,    1,   68,    2, 0x0a /* Public */,
      12,    1,   71,    2, 0x0a /* Public */,
      14,    0,   74,    2, 0x0a /* Public */,
      15,    0,   75,    2, 0x0a /* Public */,
      16,    0,   76,    2, 0x0a /* Public */,
      17,    1,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,

       0        // eod
};

void CData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receive((*reinterpret_cast< QJsonDocument(*)>(_a[1]))); break;
        case 1: _t->hasData((*reinterpret_cast< QVector<QVector3D>(*)>(_a[1])),(*reinterpret_cast< QVector<uint>(*)>(_a[2]))); break;
        case 2: _t->acceptConnection(); break;
        case 3: _t->onDataProcess((*reinterpret_cast< QJsonDocument(*)>(_a[1]))); break;
        case 4: _t->errors((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->connected(); break;
        case 6: _t->onDisconnected(); break;
        case 7: _t->readBuf(); break;
        case 8: _t->receiveDataFromDB((*reinterpret_cast< QVector<QVector<float> >(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector3D> >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<uint> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector<float> > >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CData::*)(QJsonDocument & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CData::receive)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CData::*)(QVector<QVector3D> , QVector<unsigned int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CData::hasData)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CData.data,
    qt_meta_data_CData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CData::receive(QJsonDocument & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CData::hasData(QVector<QVector3D> _t1, QVector<unsigned int> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
