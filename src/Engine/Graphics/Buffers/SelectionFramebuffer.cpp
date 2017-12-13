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
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    ShaderProgram *selectionProgram =
            ShaderProgramFactory::Get(EPATH("Shaders/G_Default.vert"),
                                      EPATH("Shaders/SelectionBuffer.frag") );

    p_selectionMaterial = Resources::Create<Material>();
    p_selectionMaterial.Get()->SetShaderProgram(selectionProgram);

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
    IdType id = 1;
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

    RH<ShaderProgram> prevSP;
    prevSP.Set( rend->GetMaterial()->GetShaderProgram() );

    ShaderProgram *selSP = p_selectionMaterial.Get()->GetShaderProgram();
    rend->GetMaterial()->SetShaderProgram(selSP);

    rend->Bind();
    selSP->Set("selectionColor", GetSelectionColor(go));
    rend->OnRender();
    rend->UnBind();

    rend->GetMaterial()->SetShaderProgram(prevSP.Get());
}

GameObject *SelectionFramebuffer::
GetGameObjectInViewportPoint(const Vector2i &vpPoint)
{
    Color colorUnderMouse = ReadColor(vpPoint.x, vpPoint.y, AttColor);
    IdType id = MapColorToId(colorUnderMouse);
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

Color SelectionFramebuffer::MapIdToColor(IdType id)
{
    constexpr IdType C = 256;
    Color color =
            Color(double(   id                % C),
                  double(  (id / C)           % C),
                  double( ((id / C) / C)      % C),
                  double((((id / C) / C) / C) % C)
                 );

    #ifdef DEBUG
        color.a = 256.0; // To be able to see selection framebuffer
    #endif

   return color / double(C);
}

typename SelectionFramebuffer::IdType
SelectionFramebuffer::MapColorToId(const Color &color)
{
    constexpr IdType C = 256;
    return   IdType(color.r * C)
           + IdType(color.g * C * C)
           + IdType(color.b * C * C * C)
           ;// + IdType(color.a * C * C * C * C);
}

RH<Texture2D> SelectionFramebuffer::GetColorTexture() const
{
    return p_colorTexture;
}
