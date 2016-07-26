#include "IInspectable.h"

#ifdef BANG_EDITOR
void IInspectable::OnInspectorInfoChanged(InspectorWidget *source)
{
}

InspectorWidgetInfo *IInspectable::OnInspectorInfoNeeded()
{
    return &m_inspectorInfo;
}
#endif
