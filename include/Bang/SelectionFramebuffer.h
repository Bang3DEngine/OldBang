#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include "Bang/Map.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Renderer;
FORWARD class Material;
FORWARD class ShaderProgram;

class SelectionFramebuffer : public Framebuffer
{
public:
    SelectionFramebuffer(int width, int height);
    virtual ~SelectionFramebuffer();

    void PrepareForRender(const Scene *scene);
    void RenderForSelectionBuffer(Renderer *renderer);

    Texture2D *GetColorTexture() const;
    GameObject *GetGameObjectInPosition(const Vector2i &screenCoords);

private:
    static const GL::Attachment AttColor = GL::Attachment::Color0;
    Texture2D *m_colorTexture = nullptr;

    Material *m_selectionMaterial = nullptr;

    mutable Map<GameObject*, long> m_gameObject_To_Id;
    mutable Map<long, GameObject*> m_id_To_GameObject;

    static Color MapIdToColor(long id);
    static long MapColorToId(const Color &color);

    Color GetSelectionColor(GameObject *go) const;

    friend class Gizmos;
    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // SELECTIONFRAMEBUFFER_H
