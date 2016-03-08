#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorPartSlotWidget : public QWidget
{
    Q_OBJECT

protected:
    InspectorPartWidget *parent;

    InspectorPartSlotWidget(InspectorPartWidget *parent);
/*
    template <class T>
    void SetValue(const T &v) {}

    template <class T>
    T GetValue() { return 0; }
*/
};

#endif // INSPECTORPARTSLOTWIDGET_H
