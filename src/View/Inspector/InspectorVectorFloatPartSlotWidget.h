#ifndef INSPECTORVECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORVECTORFLOATPARTSLOTWIDGET_H

#include <vector>

#include "InspectorPartWidget.h"
#include "InspectorFloatPartSlotWidget.h"

template <>
class InspectorPartSlotWidget< std::vector <float> >  : public InspectorPartSlotWidgetBase //Slot widget for a float vector of size N
{
    public:
        std::vector<InspectorPartSlotWidget<float>*> floatSlots;

        InspectorPartSlotWidget(InspectorPartWidget *parent,
                                const std::string &labelString,
                                std::vector<float> value);
};

#endif // INSPECTORVECTORFLOATPARTSLOTWIDGET_H
