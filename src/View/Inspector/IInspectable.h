#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "InspectorWidgetInfo.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:
    InspectorWidgetInfo m_inspectorInfo;

    //Value was changed by the user, using input in inspector
    #ifdef BANG_EDITOR
    virtual void OnInspectorInfoChanged(InspectorWidgetInfo *info);
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded();
    #endif
};

#endif // IINSPECTABLE_H
