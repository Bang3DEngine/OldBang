#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "InspectorPartInfo.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:
    InspectorPartInfo inspectorPartInfo;

    //Value was changed by the user, using input in inspector
    virtual void OnSlotValueChanged(InspectorWidget *source) = 0;

    virtual InspectorPartInfo* GetPartInfo()
    {
        return &inspectorPartInfo;
    }
};

#endif // IINSPECTABLE_H
