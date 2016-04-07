#ifndef INSPECTORVECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORVECTORFLOATPARTSLOTWIDGET_H

#include <vector>

#include "InspectorPartWidget.h"
#include "InspectorPartFloatSW.h"

class InspectorPartVFloatSW : public InspectorPartSW //Slot for a vector of size N
{
    public:
        std::vector<InspectorPartFloatSW*> floatSlots;

        InspectorPartVFloatSW(const std::string &labelString,
                              std::vector<float> initialValues,
                              InspectorWidget *parent);

        virtual void SetValue(const std::vector<float> &v);
        virtual std::vector<float> GetValue();
};

#endif // INSPECTORVECTORFLOATPARTSLOTWIDGET_H
