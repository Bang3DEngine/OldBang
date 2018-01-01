#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include "Bang/Map.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class Renderer;
FORWARD class Material;
FORWARD class GameObject;
FORWARD class ShaderProgram;

class SelectionFramebuffer : public Framebuffer
{
public:
    static const GL_Attachment AttColor = GL_Attachment::Color0;

    SelectionFramebuffer(int width, int height);
    virtual ~SelectionFramebuffer();

    void PrepareForRender(const GameObject *go);
    void RenderForSelectionBuffer(Renderer *renderer);

    RH<Texture2D> GetColorTexture() const;
    GameObject *GetGameObjectInViewportPoint(const Vector2i &vpPoint);

private:
    using IdType = unsigned long long;

    RH<Texture2D> p_colorTexture;
    RH<Material> p_selectionMaterial;

    mutable Map<GameObject*, IdType> m_gameObject_To_Id;
    mutable Map<IdType, GameObject*> m_id_To_GameObject;

    static Color MapIdToColor(IdType id);
    static IdType MapColorToId(const Color &color);

    Color GetSelectionColor(GameObject *go) const;

    friend class Gizmos;
    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // SELECTIONFRAMEBUFFER_H
