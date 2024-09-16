/****************************************************************************
** Meta object code from reading C++ file 'ros2comm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../src/include/bms_gui/ros2comm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ros2comm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Ros2Comm_t {
    QByteArrayData data[19];
    char stringdata0[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Ros2Comm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Ros2Comm_t qt_meta_stringdata_Ros2Comm = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Ros2Comm"
QT_MOC_LITERAL(1, 9, 13), // "cell1_voltage"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 13), // "cell2_voltage"
QT_MOC_LITERAL(4, 38, 13), // "cell3_voltage"
QT_MOC_LITERAL(5, 52, 13), // "cell4_voltage"
QT_MOC_LITERAL(6, 66, 13), // "cell5_voltage"
QT_MOC_LITERAL(7, 80, 13), // "cell6_voltage"
QT_MOC_LITERAL(8, 94, 10), // "lcd_cell_1"
QT_MOC_LITERAL(9, 105, 5), // "value"
QT_MOC_LITERAL(10, 111, 10), // "lcd_cell_2"
QT_MOC_LITERAL(11, 122, 10), // "lcd_cell_3"
QT_MOC_LITERAL(12, 133, 10), // "lcd_cell_4"
QT_MOC_LITERAL(13, 144, 10), // "lcd_cell_5"
QT_MOC_LITERAL(14, 155, 10), // "lcd_cell_6"
QT_MOC_LITERAL(15, 166, 10), // "lcd_cell_7"
QT_MOC_LITERAL(16, 177, 10), // "lcd_cell_8"
QT_MOC_LITERAL(17, 188, 10), // "lcd_cell_9"
QT_MOC_LITERAL(18, 199, 7) // "lcd_soc"

    },
    "Ros2Comm\0cell1_voltage\0\0cell2_voltage\0"
    "cell3_voltage\0cell4_voltage\0cell5_voltage\0"
    "cell6_voltage\0lcd_cell_1\0value\0"
    "lcd_cell_2\0lcd_cell_3\0lcd_cell_4\0"
    "lcd_cell_5\0lcd_cell_6\0lcd_cell_7\0"
    "lcd_cell_8\0lcd_cell_9\0lcd_soc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Ros2Comm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       3,    1,   97,    2, 0x06 /* Public */,
       4,    1,  100,    2, 0x06 /* Public */,
       5,    1,  103,    2, 0x06 /* Public */,
       6,    1,  106,    2, 0x06 /* Public */,
       7,    1,  109,    2, 0x06 /* Public */,
       8,    1,  112,    2, 0x06 /* Public */,
      10,    1,  115,    2, 0x06 /* Public */,
      11,    1,  118,    2, 0x06 /* Public */,
      12,    1,  121,    2, 0x06 /* Public */,
      13,    1,  124,    2, 0x06 /* Public */,
      14,    1,  127,    2, 0x06 /* Public */,
      15,    1,  130,    2, 0x06 /* Public */,
      16,    1,  133,    2, 0x06 /* Public */,
      17,    1,  136,    2, 0x06 /* Public */,
      18,    1,  139,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void Ros2Comm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Ros2Comm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->cell1_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->cell2_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->cell3_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->cell4_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->cell5_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->cell6_voltage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->lcd_cell_1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->lcd_cell_2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->lcd_cell_3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->lcd_cell_4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->lcd_cell_5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->lcd_cell_6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->lcd_cell_7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->lcd_cell_8((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->lcd_cell_9((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->lcd_soc((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell1_voltage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell2_voltage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell3_voltage)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell4_voltage)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell5_voltage)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::cell6_voltage)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_1)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_2)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_3)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_4)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_5)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_6)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_7)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_8)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_cell_9)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Ros2Comm::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ros2Comm::lcd_soc)) {
                *result = 15;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Ros2Comm::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Ros2Comm.data,
    qt_meta_data_Ros2Comm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Ros2Comm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ros2Comm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Ros2Comm.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Ros2Comm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void Ros2Comm::cell1_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Ros2Comm::cell2_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Ros2Comm::cell3_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Ros2Comm::cell4_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Ros2Comm::cell5_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Ros2Comm::cell6_voltage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Ros2Comm::lcd_cell_1(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Ros2Comm::lcd_cell_2(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Ros2Comm::lcd_cell_3(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Ros2Comm::lcd_cell_4(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Ros2Comm::lcd_cell_5(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Ros2Comm::lcd_cell_6(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Ros2Comm::lcd_cell_7(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Ros2Comm::lcd_cell_8(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Ros2Comm::lcd_cell_9(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Ros2Comm::lcd_soc(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
