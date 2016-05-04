#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "InspectorWidgetInfo.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:
    InspectorWidgetInfo inspectorComponentInfo;

    //Value was changed by the user, using input in inspector
    virtual void OnSlotValueChanged(InspectorWidget *source) = 0;

    virtual InspectorWidgetInfo* GetComponentInfo()
    {
        return &inspectorComponentInfo;
    }
};

#endif // IINSPECTABLE_H
