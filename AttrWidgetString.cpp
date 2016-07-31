/****************************************************************************
** Meta object code from reading C++ file 'AttrWidgetString.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/View/Inspector/AttrWidgetString.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AttrWidgetString.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AttrWidgetString[] = {

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

static const char qt_meta_stringdata_AttrWidgetString[] = {
    "AttrWidgetString\0"
};

void AttrWidgetString::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData AttrWidgetString::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AttrWidgetString::staticMetaObject = {
    { &AttributeWidget::staticMetaObject, qt_meta_stringdata_AttrWidgetString,
      qt_meta_data_AttrWidgetString, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AttrWidgetString::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AttrWidgetString::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AttrWidgetString::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AttrWidgetString))
        return static_cast<void*>(const_cast< AttrWidgetString*>(this));
    return AttributeWidget::qt_metacast(_clname);
}

int AttrWidgetString::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AttributeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_LabelStringSW[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LabelStringSW[] = {
    "LabelStringSW\0\0SelectAll()\0"
};

void LabelStringSW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LabelStringSW *_t = static_cast<LabelStringSW *>(_o);
        switch (_id) {
        case 0: _t->SelectAll(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LabelStringSW::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LabelStringSW::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_LabelStringSW,
      qt_meta_data_LabelStringSW, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LabelStringSW::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LabelStringSW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LabelStringSW::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LabelStringSW))
        return static_cast<void*>(const_cast< LabelStringSW*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int LabelStringSW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
