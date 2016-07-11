#ifndef INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
#define INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H

#include <vector>

#include "InspectorComponentWidget.h"
#include "InspectorFloatSW.h"

class InspectorVFloatSW : public InspectorSW //Slot for a vector of size N
{
    public:
        std::vector<InspectorFloatSW*> m_floatSlots;

        InspectorVFloatSW(const std::string &labelString,
                              std::vector<float> initialValues,
                              InspectorWidget *p_parent);

        virtual void SetValue(const std::vector<float> &v);
        virtual std::vector<float> GetValue();
};

#endif // INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
