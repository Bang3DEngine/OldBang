#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorPartSlotWidget : public QWidget
{
protected:
    InspectorPartWidget *parent;

    InspectorPartSlotWidget(InspectorPartWidget *parent);
    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;
};

#endif // INSPECTORPARTSLOTWIDGET_H
