#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "XMLNode.h"

class InspectorWidget;
class IInspectable
{
protected:
    IInspectable() {}

public:

    virtual ~IInspectable() {}

    #ifdef BANG_EDITOR
    // Value was changed by the user, using input in inspector
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo);

    // Inspector want to update its values
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const;
    #endif
};

#endif // IINSPECTABLE_H
