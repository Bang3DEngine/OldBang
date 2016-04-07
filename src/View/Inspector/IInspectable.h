#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "InspectorWidgetInfo.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:
    InspectorWidgetInfo inspectorPartInfo;

    //Value was changed by the user, using input in inspector
    virtual void OnSlotValueChanged(InspectorWidget *source) = 0;

    virtual InspectorWidgetInfo* GetPartInfo()
    {
        return &inspectorPartInfo;
    }
};

#endif // IINSPECTABLE_H
