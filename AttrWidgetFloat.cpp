/****************************************************************************
** Meta object code from reading C++ file 'AttrWidgetFloat.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/View/Inspector/AttrWidgetFloat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AttrWidgetFloat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AttrWidgetFloat[] = {

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

static const char qt_meta_stringdata_AttrWidgetFloat[] = {
    "AttrWidgetFloat\0"
};

void AttrWidgetFloat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData AttrWidgetFloat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AttrWidgetFloat::staticMetaObject = {
    { &AttributeWidget::staticMetaObject, qt_meta_stringdata_AttrWidgetFloat,
      qt_meta_data_AttrWidgetFloat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AttrWidgetFloat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AttrWidgetFloat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AttrWidgetFloat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AttrWidgetFloat))
        return static_cast<void*>(const_cast< AttrWidgetFloat*>(this));
    return AttributeWidget::qt_metacast(_clname);
}

int AttrWidgetFloat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AttributeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_FloatComponentSlotSpinBox[] = {

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
      29,   27,   26,   26, 0x0a,
      48,   26,   26,   26, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FloatComponentSlotSpinBox[] = {
    "FloatComponentSlotSpinBox\0\0v\0"
    "AdjustStep(double)\0SelectAll()\0"
};

void FloatComponentSlotSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FloatComponentSlotSpinBox *_t = static_cast<FloatComponentSlotSpinBox *>(_o);
        switch (_id) {
        case 0: _t->AdjustStep((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->SelectAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FloatComponentSlotSpinBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FloatComponentSlotSpinBox::staticMetaObject = {
    { &QDoubleSpinBox::staticMetaObject, qt_meta_stringdata_FloatComponentSlotSpinBox,
      qt_meta_data_FloatComponentSlotSpinBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatComponentSlotSpinBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatComponentSlotSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatComponentSlotSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatComponentSlotSpinBox))
        return static_cast<void*>(const_cast< FloatComponentSlotSpinBox*>(this));
    return QDoubleSpinBox::qt_metacast(_clname);
}

int FloatComponentSlotSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
