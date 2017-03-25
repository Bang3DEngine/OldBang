#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Color.h"
#include "String.h"
#include "Matrix4.h"
#include "MeshRenderer.h"

class XMLNode;
class Material;
class ICloneable;
class UIRenderer : public MeshRenderer
{
    OBJECT(UIRenderer)
    ICLONEABLE(UIRenderer)

public:
    virtual void RenderWithoutMaterial() const override;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Color m_tint = Color::White;

    UIRenderer();
    virtual ~UIRenderer();

    void SetTint(const Color &tint);
    const Color& GetTint() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void CloneInto(ICloneable *clone) const override;
};

#endif // UIRENDERER_H
