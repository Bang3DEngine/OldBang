#include "Bang/Material.h"

#include "Bang/IO.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Shader.h"
#include "Bang/Camera.h"
#include "Bang/Vector4.h"
#include "Bang/Texture.h"
#include "Bang/Texture2D.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GraphicPipeline.h"

Material::Material() : Asset()
{
    SetShaderProgram( new ShaderProgram(
                          IO::ToAbsolute("Shaders/G_Default.vert_g", true),
                          IO::ToAbsolute("Shaders/G_Default.frag_g", true)));
}

Material::Material(const Material &m)
{
    ShaderProgram *sp = m.GetShaderProgram();
    if (sp && sp->GetVertexShader() && sp->GetFragmentShader())
    {
        Shader *vshader = sp->GetVertexShader();
        Shader *fshader = sp->GetFragmentShader();
        SetShaderProgram( new ShaderProgram(vshader->GetFilepath(),
                                            fshader->GetFilepath()));
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

    String texAssetFilepath = xmlInfo.GetString("Texture");
    Texture2D *texture = AssetsManager::Load<Texture2D>(texAssetFilepath);
    SetTexture(texture);

    ShaderProgram *sp = GetShaderProgram();
    String vshaderFilepath = xmlInfo.GetFilepath("VertexShader");
    String fshaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
        vshaderFilepath != sp->GetVertexShader()->GetFilepath() ||
        fshaderFilepath != sp->GetFragmentShader()->GetFilepath()
       )
    {
        SetShaderProgram(new ShaderProgram(vshaderFilepath,
                                           fshaderFilepath));
    }
}

void Material::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->SetColor("DiffuseColor",    GetDiffuseColor());
    xmlInfo->SetFloat("Shininess",       GetShininess());
    xmlInfo->SetBool("ReceivesLighting", ReceivesLighting());
    xmlInfo->SetVector2("UvMultiply",    GetUvMultiply());

    String textureFilepath = GetTexture() ? GetTexture()->GetFilepath() : "";
    xmlInfo->SetFilepath("Texture", textureFilepath,
                         Texture2D::GetFileExtensionStatic());

    String vsFile =  "", fsFile = "";
    ShaderProgram *sp = GetShaderProgram();
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
    return AssetsManager::Load<Material>("./Materials/Missing.bmat", true);
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    m_shaderProgram = program;
}

void Material::SetTexture(const Texture2D *texture)
{
    m_texture = texture;
    if (m_texture)
    {
        ShaderProgram *sp = GetShaderProgram();
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

ShaderProgram *Material::GetShaderProgram() const
{
    ShaderProgram *camReplacementSP = nullptr;
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
        ShaderProgram *defaultSP = nullptr;
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
    ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->Bind();

    GBuffer *gb = GraphicPipeline::GetActive()->GetGBuffer();
    gb->BindTextureBuffersTo(sp, false);

    sp->SetVec2("B_ScreenSize", Screen::GetSize());

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
    ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->UnBind();
}
