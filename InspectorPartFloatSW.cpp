/****************************************************************************
** Meta object code from reading C++ file 'InspectorPartFloatSW.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/View/Inspector/InspectorPartFloatSW.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InspectorPartFloatSW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InspectorPartFloatSW[] = {

 // content:
       6,       // revision
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

static const char qt_meta_stringdata_InspectorPartFloatSW[] = {
    "InspectorPartFloatSW\0"
};

void InspectorPartFloatSW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData InspectorPartFloatSW::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InspectorPartFloatSW::staticMetaObject = {
    { &InspectorPartSW::staticMetaObject, qt_meta_stringdata_InspectorPartFloatSW,
      qt_meta_data_InspectorPartFloatSW, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InspectorPartFloatSW::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InspectorPartFloatSW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InspectorPartFloatSW::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InspectorPartFloatSW))
        return static_cast<void*>(const_cast< InspectorPartFloatSW*>(this));
    return InspectorPartSW::qt_metacast(_clname);
}

int InspectorPartFloatSW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InspectorPartSW::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_FloatPartSlotSpinBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   22,   21,   21, 0x0a,
      43,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FloatPartSlotSpinBox[] = {
    "FloatPartSlotSpinBox\0\0v\0AdjustStep(double)\0"
    "SelectAll()\0"
};

void FloatPartSlotSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FloatPartSlotSpinBox *_t = static_cast<FloatPartSlotSpinBox *>(_o);
        switch (_id) {
        case 0: _t->AdjustStep((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->SelectAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FloatPartSlotSpinBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FloatPartSlotSpinBox::staticMetaObject = {
    { &QDoubleSpinBox::staticMetaObject, qt_meta_stringdata_FloatPartSlotSpinBox,
      qt_meta_data_FloatPartSlotSpinBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatPartSlotSpinBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatPartSlotSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatPartSlotSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatPartSlotSpinBox))
        return static_cast<void*>(const_cast< FloatPartSlotSpinBox*>(this));
    return QDoubleSpinBox::qt_metacast(_clname);
}

int FloatPartSlotSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDoubleSpinBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
