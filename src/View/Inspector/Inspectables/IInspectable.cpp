#include "IInspectable.h"

#include "XMLNode.h"

#ifdef BANG_EDITOR
void IInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
}

void IInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
}
#endif
