#ifndef INSPECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORFLOATPARTSLOTWIDGET_H

#include <QDoubleSpinBox>

#include "InspectorPartSlotWidget.h"

class InspectorFloatPartSlotWidget : public InspectorPartSlotWidget //Slot for a float (label + float)
{
    private:
        QDoubleSpinBox *spinbox;

    public:
        InspectorFloatPartSlotWidget(float initialValue, const std::string &labelString,
                                     InspectorPartWidget *parent);

        virtual void SetValue(float f);
        virtual float GetValue();

        virtual void focusInEvent(QFocusEvent * event) override;
        virtual void focusOutEvent(QFocusEvent * event) override;
        QSize sizeHint() const override;
};

#endif // INSPECTORFLOATPARTSLOTWIDGET_H
