#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "UIRenderer.h"

class UIImage : public UIRenderer
{
public:
    UIImage();
    virtual ~UIImage();

    virtual const String ToString() const override;
    virtual String GetName() const override;
    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomPR() const override;

    friend class GameObject;
};

#endif // UIIMAGE_H
