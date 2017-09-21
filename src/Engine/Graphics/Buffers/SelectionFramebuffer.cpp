#include "Bang/SelectionFramebuffer.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Input.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    ShaderProgram *selectionProgram = new ShaderProgram();
    selectionProgram->Load( EPATH("Shaders/G_Default.vert_g"),
                            EPATH("Shaders/SelectionBuffer.frag_sel") );

    m_selectionMaterial = new Material();
    m_selectionMaterial->SetShaderProgram(selectionProgram);

    Bind();
    CreateAttachment(AttColor, GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(GL::Attachment::DepthStencil,
                     GL::ColorFormat::Depth24_Stencil8);
    UnBind();

    m_colorTexture = GetAttachmentTexture(AttColor);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete m_selectionMaterial;
}

void SelectionFramebuffer::PrepareForRender(const Scene *scene)
{
    int id = 1;
    m_gameObject_To_Id.Clear();
    m_id_To_GameObject.Clear();

    List<GameObject*> gameObjects = scene->GetChildrenRecursively();
    for (GameObject *go : gameObjects)
    {
        m_gameObject_To_Id[go] = id;
        m_id_To_GameObject[id] = go;
        ++id;
    }

    SetAllColorDrawBuffers();
}

void SelectionFramebuffer::RenderForSelectionBuffer(Renderer *rend)
{
    ASSERT(GL::IsBound(this));

    GameObject *go = rend->gameObject;

    ShaderProgram *selSP = m_selectionMaterial->GetShaderProgram();
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

Texture2D *SelectionFramebuffer::GetColorTexture() const
{
    return m_colorTexture;
}
