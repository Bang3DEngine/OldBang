#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Bang/Color.h"
#include "Bang/MeshRenderer.h"

class UIRenderer : public MeshRenderer
{
    COMPONENT(UIRenderer)

public:
    virtual void Render() const override;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    void SetTint(const Color &tint);
    const Color& GetTint() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Color m_tint = Color::White;

    UIRenderer();
    virtual ~UIRenderer();

    virtual void CloneInto(ICloneable *clone) const override;
};

#endif // UIRENDERER_H
