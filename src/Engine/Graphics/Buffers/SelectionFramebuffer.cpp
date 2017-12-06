#include "Bang/SelectionFramebuffer.h"

#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    RH<ShaderProgram> selectionProgram;
    Resources::Create<ShaderProgram>(&selectionProgram,
                                     EPATH("Shaders/G_Default.vert"),
                                     EPATH("Shaders/SelectionBuffer.frag") );

    Resources::Create<Material>(&p_selectionMaterial);
    p_selectionMaterial.Get()->SetShaderProgram(selectionProgram.Get());

    Bind();
    CreateAttachment(AttColor, GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(GL::Attachment::DepthStencil,
                     GL::ColorFormat::Depth24_Stencil8);
    UnBind();

    p_colorTexture.Set(GetAttachmentTexture(AttColor));
}

SelectionFramebuffer::~SelectionFramebuffer()
{
}

void SelectionFramebuffer::PrepareForRender(const GameObject *go)
{
    int id = 1;
    m_gameObject_To_Id.Clear();
    m_id_To_GameObject.Clear();

    List<GameObject*> gameObjects = go->GetChildrenRecursively();
    for (GameObject *go : gameObjects)
    {
        m_gameObject_To_Id[go] = id;
        m_id_To_GameObject[id] = go;
        ++id;
    }

    SetAllDrawBuffers();
}

void SelectionFramebuffer::RenderForSelectionBuffer(Renderer *rend)
{
    ASSERT(GL::IsBound(this));

    GameObject *go = rend->GetGameObject();

    ShaderProgram *selSP = p_selectionMaterial.Get()->GetShaderProgram();
    ShaderProgram *prevSP = rend->GetMaterial()->GetShaderProgram();
    rend->GetMaterial()->SetShaderProgram(selSP);

    rend->Bind();
    selSP->Set("selectionColor", GetSelectionColor(go));
    rend->OnRender();
    rend->UnBind();

    rend->GetMaterial()->SetShaderProgram(prevSP);
}

GameObject *SelectionFramebuffer::GetGameObjectInPosition(
        const Vector2i &screenCoords)
{
    Color colorUnderMouse = ReadColor(screenCoords.x, screenCoords.y, AttColor);
    long id = MapColorToId(colorUnderMouse);
    if (colorUnderMouse != Color::Zero && m_id_To_GameObject.ContainsKey(id))
    {
        return m_id_To_GameObject[id];
    }
    return nullptr;
}

Color SelectionFramebuffer::GetSelectionColor(GameObject *go) const
{
    return MapIdToColor(m_gameObject_To_Id[go]);
}

Color SelectionFramebuffer::MapIdToColor(long id)
{
    constexpr int C = 256;
    Color color =
            Color(float(   id                % C),
                  float(  (id / C)           % C),
                  float( ((id / C) / C)      % C),
                  float((((id / C) / C) / C) % C)
                 );
   return color / float(C);
}

long SelectionFramebuffer::MapColorToId(const Color &color)
{
    constexpr int C = 256;
    return long(color.r * C) +
           long(color.g * C * C) +
           long(color.b * C * C * C) +
           long(color.a * C * C * C * C);
}

RH<Texture2D> SelectionFramebuffer::GetColorTexture() const
{
    return p_colorTexture;
}
