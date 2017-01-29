#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "String.h"
#include "UIRenderer.h"

class XMLNode;
class ICloneable;
class UIImage : public UIRenderer
{
public:
    UIImage();
    virtual ~UIImage();

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomPR() const override;

    friend class GameObject;
};

#endif // UIIMAGE_H
