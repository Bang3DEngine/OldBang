#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "InspectorWidgetInfo.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:
    InspectorWidgetInfo m_inspectorComponentInfo;

    //Value was changed by the user, using input in inspector
    virtual void OnSlotValueChanged(InspectorWidget *source) = 0;

    virtual InspectorWidgetInfo* GetComponentInfo()
    {
        return &m_inspectorComponentInfo;
    }
};

#endif // IINSPECTABLE_H
