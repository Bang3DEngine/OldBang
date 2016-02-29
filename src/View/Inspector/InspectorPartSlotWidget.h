#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorPartSlotWidget : public QWidget
{
protected:
    InspectorPartWidget *parent;

    InspectorPartSlotWidget(InspectorPartWidget *parent);

    template <class T>
    void SetValue(const T &v) {}

    template <class T>
    T GetValue() { return nullptr; }
};

#endif // INSPECTORPARTSLOTWIDGET_H
