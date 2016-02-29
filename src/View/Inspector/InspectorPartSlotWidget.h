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

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;

    template <class T>
    void SetValue(const T &v) {}

    template <class T>
    T GetValue() { return nullptr; }
};

#endif // INSPECTORPARTSLOTWIDGET_H
