#ifndef INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
#define INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H

#include "Array.h"

#include "ComponentWidget.h"
#include "AttrWidgetFloat.h"

class AttrWidgetVectorFloat : public AttributeWidget //Slot for a vector of size N
{
public:
    Array<AttrWidgetFloat*> m_floatSlots;

    AttrWidgetVectorFloat(const XMLAttribute &xmlAttribute,
                          InspectorWidget *m_inspectorWidget);

    virtual void SetValue(const Array<float> &v);
    virtual Array<float> GetValue();
    float GetFloat() const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

    virtual void Refresh(const XMLAttribute &attribute) override;
};

#endif // INSPECTORVECTORFLOATCOMPONENTSLOTWIDGET_H
