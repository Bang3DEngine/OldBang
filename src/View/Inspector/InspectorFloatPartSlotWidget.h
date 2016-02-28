#ifndef INSPECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORFLOATPARTSLOTWIDGET_H

#include <QDoubleSpinBox>

#include "InspectorPartSlotWidget.h"

class InspectorFloatPartSlotWidget : public InspectorPartSlotWidget //Slot for a float (label + float)
{
    public:
        QDoubleSpinBox *spinbox;

        InspectorFloatPartSlotWidget(float initialValue, const std::string &labelString,InspectorPartWidget *parent);
        virtual void SetValue(float f);
        virtual float GetValue();

        QSize sizeHint() const override;
};

#endif // INSPECTORFLOATPARTSLOTWIDGET_H
