/****************************************************************************
** Meta object code from reading C++ file 'ComponentWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/View/Inspector/ComponentWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ComponentWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ComponentWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      34,   26,   25,   25, 0x0a,
      65,   25,   25,   25, 0x0a,
     104,   25,   25,   25, 0x0a,
     134,   25,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ComponentWidget[] = {
    "ComponentWidget\0\0checked\0"
    "OnEnabledCheckboxPressed(bool)\0"
    "OnContextMenuRemoveComponentSelected()\0"
    "OnContextMenuMoveUpSelected()\0"
    "OnContextMenuMoveDownSelected()\0"
};

void ComponentWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ComponentWidget *_t = static_cast<ComponentWidget *>(_o);
        switch (_id) {
        case 0: _t->OnEnabledCheckboxPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->OnContextMenuRemoveComponentSelected(); break;
        case 2: _t->OnContextMenuMoveUpSelected(); break;
        case 3: _t->OnContextMenuMoveDownSelected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ComponentWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ComponentWidget::staticMetaObject = {
    { &InspectorWidget::staticMetaObject, qt_meta_stringdata_ComponentWidget,
      qt_meta_data_ComponentWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ComponentWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ComponentWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ComponentWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentWidget))
        return static_cast<void*>(const_cast< ComponentWidget*>(this));
    return InspectorWidget::qt_metacast(_clname);
}

int ComponentWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InspectorWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
