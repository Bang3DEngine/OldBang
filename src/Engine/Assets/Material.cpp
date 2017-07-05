#include "Bang/Material.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Vector4.h"
#include "Bang/G_Screen.h"
#include "Bang/G_Shader.h"
#include "Bang/G_Texture.h"
#include "Bang/Texture2D.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GraphicPipeline.h"

Material::Material() : Asset()
{
    ShaderProgram *sp = new ShaderProgram();
    sp->Load(EPATH("Shaders/G_Default.vert_g"),
             EPATH("Shaders/G_Default.frag_g"));
    SetShaderProgram(sp);
}

Material::Material(const Material &m)
{
    G_ShaderProgram *sp = m.GetShaderProgram();
    if (sp && sp->GetVertexShader() && sp->GetFragmentShader())
    {
        G_Shader *vshader = sp->GetVertexShader();
        G_Shader *fshader = sp->GetFragmentShader();
        ShaderProgram *sp = new ShaderProgram();
        sp->Load(vshader->GetFilepath(), fshader->GetFilepath());
        SetShaderProgram(sp);
    }

    SetDiffuseColor(m.GetDiffuseColor());
    SetReceivesLighting(m.ReceivesLighting());
    SetShininess(m.GetShininess());
    SetTexture(m.GetTexture());
}

Material::~Material()
{
    if (m_shaderProgram) { delete m_shaderProgram; }
}

String Material::GetFileExtensionStatic()
{
    return "bmat";
}

String Material::GetFileExtension() const
{
    return Material::GetFileExtensionStatic();
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_uvMultiply = uvMultiply;
}

void Material::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    SetDiffuseColor(xmlInfo.GetColor("DiffuseColor"));
    SetShininess(xmlInfo.GetFloat("Shininess"));
    SetReceivesLighting(xmlInfo.GetBool("ReceivesLighting"));
    SetUvMultiply(xmlInfo.GetVector2("UvMultiply"));

    Path texAssetFilepath = xmlInfo.GetFilepath("Texture");
    Texture2D *texture = AssetsManager::Load<Texture2D>(texAssetFilepath);
    SetTexture(texture);

    G_ShaderProgram *sp = GetShaderProgram();
    Path vshaderFilepath = xmlInfo.GetFilepath("VertexShader");
    Path fshaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
        vshaderFilepath != sp->GetVertexShader()->GetFilepath() ||
        fshaderFilepath != sp->GetFragmentShader()->GetFilepath()
       )
    {
        ShaderProgram *sp = new ShaderProgram();
        sp->Load(vshaderFilepath, fshaderFilepath);
        SetShaderProgram(sp);
    }
}

void Material::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->SetColor("DiffuseColor",    GetDiffuseColor());
    xmlInfo->SetFloat("Shininess",       GetShininess());
    xmlInfo->SetBool("ReceivesLighting", ReceivesLighting());
    xmlInfo->SetVector2("UvMultiply",    GetUvMultiply());

    Path texFilepath = GetTexture() ? GetTexture()->GetFilepath() : Path();
    xmlInfo->SetFilepath("Texture", texFilepath,
                         Texture2D::GetFileExtensionStatic());

    Path vsFile, fsFile;
    G_ShaderProgram *sp = GetShaderProgram();
    if (sp)
    {
        if (sp->GetVertexShader())
        {
            vsFile = sp->GetVertexShader()->GetFilepath();
        }

        if (sp->GetFragmentShader())
        {
            fsFile = sp->GetFragmentShader()->GetFilepath();
        }
    }
    xmlInfo->SetFilepath("VertexShader", vsFile, "vert_g vert_pp vert_sel vert");
    xmlInfo->SetFilepath("FragmentShader", fsFile, "frag_g frag_pp frag_sel frag");
}

Material *Material::GetMissingMaterial()
{
    return AssetsManager::Load<Material>( EPATH("Materials/Missing.bmat") );
}

void Material::SetShaderProgram(G_ShaderProgram *program)
{
    m_shaderProgram = program;
}

void Material::SetTexture(const Texture2D *texture)
{
    m_texture = texture;
    if (m_texture)
    {
        G_ShaderProgram *sp = GetShaderProgram();
        sp->SetTexture("B_Texture0", m_texture);
    }
}

void Material::SetReceivesLighting(bool receivesLighting)
{
    m_receivesLighting = receivesLighting;
}

void Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

void Material::SetDiffuseColor(const Color &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

const Vector2 &Material::GetUvMultiply() const
{
    return m_uvMultiply;
}

G_ShaderProgram *Material::GetShaderProgram() const
{
    G_ShaderProgram *camReplacementSP = nullptr;
    Scene *scene = SceneManager::GetActiveScene();
    if (scene)
    {
        Camera *cam = scene->GetCamera();
        if (cam) { camReplacementSP = cam->GetReplacementShaderProgram(); }
    }

    if (camReplacementSP)
    {
        return camReplacementSP;
    }
    else if (!m_shaderProgram)
    {
        G_ShaderProgram *defaultSP = nullptr;
        defaultSP = Material::GetMissingMaterial()->GetShaderProgram();
        return defaultSP;
    }
    return m_shaderProgram;
}

const Texture2D *Material::GetTexture() const
{
    return m_texture;
}

bool Material::ReceivesLighting() const
{
    return m_receivesLighting;
}

float Material::GetShininess() const
{
    return m_shininess;
}

const Color& Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}

void Material::Bind() const
{
    G_ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->Bind();

    G_GBuffer *gb = GraphicPipeline::GetActive()->GetG_GBuffer();
    gb->BindTextureBuffersTo(sp, false);

    sp->SetVec2("B_ScreenSize", G_Screen::GetSize());

    sp->SetVec2( "B_UvMultiply",               GetUvMultiply());
    sp->SetColor("B_MaterialDiffuseColor",     GetDiffuseColor());
    sp->SetFloat("B_MaterialShininess",        GetShininess());
    sp->SetFloat("B_MaterialReceivesLighting", ReceivesLighting() ? 1.0f : 0.0f);

    float alphaCutoff = GetTexture() ? GetTexture()->GetAlphaCutoff() : -1.0f;
    sp->SetTexture("B_Texture0",  GetTexture());
    sp->SetFloat("B_AlphaCutoff", alphaCutoff);
    sp->SetBool("B_HasTexture",   GetTexture() != nullptr);

    GL::ApplyContextToShaderProgram(sp);
}

void Material::UnBind() const
{
    G_ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->UnBind();
}
