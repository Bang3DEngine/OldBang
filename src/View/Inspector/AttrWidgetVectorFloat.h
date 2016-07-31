#ifndef INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
#define INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H

#include <vector>

#include "ComponentWidget.h"
#include "AttrWidgetFloat.h"

class AttrWidgetVectorFloat : public AttributeWidget //Slot for a vector of size N
{
    public:
        std::vector<AttrWidgetFloat*> m_floatSlots;

        AttrWidgetVectorFloat(const std::string &labelString,
                              int numberOfFields,
                              InspectorWidget *m_parent);

        virtual void SetValue(const std::vector<float> &v);
        virtual std::vector<float> GetValue();
        float GetFloat() const;
        glm::vec2 GetVector2() const;
        Vector3 GetVector3() const;
        glm::vec4 GetVector4() const;
};

#endif // INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
