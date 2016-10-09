#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "MeshRenderer.h"

class UIRenderer : public MeshRenderer
{
public:
    virtual void RenderCustomPR() const override;
    bool IsACanvasRenderer() const override;

    virtual void SetMatricesUniforms(
            Material *mat,
            const Matrix4 &model,
            const Matrix4 &normal,
            const Matrix4 &view,
            const Matrix4 &projection,
            const Matrix4 &pvm) const override;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    virtual const String ToString() const override;
    virtual String GetName() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override = 0;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override = 0;

protected:
    Material *m_materialPR = new Material();
    Color m_tint = Color::Zero;
    Color m_strokeColor = Color::White;
    float m_stroke = 0.0f;

    UIRenderer();
    virtual ~UIRenderer();

    friend class GameObject;
};

#endif // UIRENDERER_H
