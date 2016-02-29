#ifndef INSPECTORVECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORVECTORFLOATPARTSLOTWIDGET_H

#include <vector>

#include "InspectorPartWidget.h"
#include "InspectorFloatPartSlotWidget.h"

class InspectorVectorFloatPartSlotWidget : public InspectorPartSlotWidget //Slot for a vector of size N
{
    public:
        std::vector<InspectorFloatPartSlotWidget*> floatSlots;

        InspectorVectorFloatPartSlotWidget(std::vector<float> initialValues, const std::string &labelString,
                                           InspectorPartWidget *parent);

        virtual void SetValue(const std::vector<float> &v);
        virtual std::vector<float> GetValue();
};

#endif // INSPECTORVECTORFLOATPARTSLOTWIDGET_H
