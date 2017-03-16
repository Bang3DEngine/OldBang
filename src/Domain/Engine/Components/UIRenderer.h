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
    ICLONEABLE(UIRenderer)

public:
    virtual void RenderWithoutMaterial() const override;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    virtual String GetName() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Material *m_materialSP = nullptr;
    Color m_tint = Color::White;

    UIRenderer();
    virtual ~UIRenderer();

    virtual void OnJustBeforeRendering(GameObject *go,
                                       ShaderProgram *sp) override;

    virtual void CloneInto(ICloneable *clone) const override;

    friend class GameObject;
};

#endif // UIRENDERER_H
