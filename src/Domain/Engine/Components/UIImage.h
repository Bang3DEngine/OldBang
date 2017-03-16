#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "String.h"
#include "Texture2D.h"
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
    ICloneable *CloneVirtual() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetTexture(Texture2D *imageTexture);

    Texture2D *GetTexture() const;

private:
    Texture2D *m_imageTexture = nullptr;

    virtual void OnJustBeforeRendering(GameObject *go,
                                       ShaderProgram *sp) override;

    friend class GameObject;
};

#endif // UIIMAGE_H
