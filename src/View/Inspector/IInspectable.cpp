#include "IInspectable.h"

#ifdef BANG_EDITOR
void IInspectable::OnInspectorInfoChanged(InspectorWidgetInfo *info)
{
}

InspectorWidgetInfo *IInspectable::OnInspectorInfoNeeded()
{
    return &m_inspectorInfo;
}
#endif
