#ifndef INSPECTORVECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORVECTORFLOATPARTSLOTWIDGET_H

#include <vector>

#include "InspectorPartWidget.h"
#include "InspectorFloatSW.h"

class InspectorVFloatSW : public InspectorSW //Slot for a vector of size N
{
    public:
        std::vector<InspectorFloatSW*> floatSlots;

        InspectorVFloatSW(const std::string &labelString,
                              std::vector<float> initialValues,
                              InspectorWidget *parent);

        virtual void SetValue(const std::vector<float> &v);
        virtual std::vector<float> GetValue();
};

#endif // INSPECTORVECTORFLOATPARTSLOTWIDGET_H
