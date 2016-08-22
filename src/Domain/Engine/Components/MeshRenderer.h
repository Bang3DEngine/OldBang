#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang.h"

#include <glm/gtx/string_cast.hpp>

#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "Renderer.h"

#ifdef BANG_EDITOR
#include "AttrWidgetFile.h"
#endif


class MeshRenderer : public Renderer
{
friend class GameObject;

private:
    Mesh *m_mesh = nullptr;

protected:

    virtual void RenderWithoutBindingMaterial() const override;

    MeshRenderer();

public:

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;
    virtual ~MeshRenderer();

    void SetMaterial(Material *m) override;
    virtual void SetMesh(Mesh *m);

    virtual Box GetBoundingBox() const override;

    virtual const Mesh* GetMesh();
    virtual const String ToString() const override;
    virtual String GetName() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // MESHRENDERER_H
