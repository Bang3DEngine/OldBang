#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>
#include "WindowEventManager.h"

class InspectorPartSlotWidgetBase : public QWidget
{
protected:
    InspectorPartSlotWidgetBase() : QWidget() {}
    virtual ~InspectorPartSlotWidgetBase() {}
};

class InspectorPartWidget;

template<class T>
class InspectorPartSlotWidget : public InspectorPartSlotWidgetBase
{
private:
    T value;

protected:
    InspectorPartWidget *parent = nullptr;

    InspectorPartSlotWidget(InspectorPartWidget *parent,
                            const std::string &label,
                            const T &value) : parent(parent), value(value)
    {}

public:

    void SetValue(const T &v) { this->value = v; }
    T* GetValue() { return &value; }
};

#endif // INSPECTORPARTSLOTWIDGET_H
