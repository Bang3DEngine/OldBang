#include "Material.h"

#include "Scene.h"
#include "Screen.h"
#include "Shader.h"
#include "Camera.h"
#include "Vector4.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"

#include "AssetsManager.h"

Material::Material() : Asset()
{
    // TODO: Create shaderProgram asset and use AssetManager to load this
    SetShaderProgram(new ShaderProgram(
                      Persistence::ToAbsolute("Shaders/G_Default.vert", true),
                      Persistence::ToAbsolute("Shaders/G_Default.frag", true)
                     )
                    );
}

Material::Material(const Material &m)
{
    ShaderProgram *sp = m.GetShaderProgram();
    SetShaderProgram(new ShaderProgram(sp->GetVertexShader()->GetFilepath(),
                                       sp->GetFragmentShader()->GetFilepath()));
    SetDiffuseColor(m.GetDiffuseColor());
    SetReceivesLighting(m.ReceivesLighting());
    SetShininess(m.GetShininess());
    SetTexture(m.GetTexture());
}

Material::~Material()
{
    if (m_shaderProgram)
    {
        delete m_shaderProgram;
    }
}

String Material::GetFileExtensionStatic()
{
    return "bmat";
}

String Material::GetFileExtension()
{
    return Material::GetFileExtensionStatic();
}

void Material::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    SetDiffuseColor(xmlInfo->GetColor("DiffuseColor"));
    SetShininess(xmlInfo->GetFloat("Shininess"));
    SetReceivesLighting(xmlInfo->GetBool("ReceivesLighting"));

    String texAssetFilepath = xmlInfo->GetString("Texture");
    Texture2D *texture = AssetsManager::Load<Texture2D>(texAssetFilepath);
    SetTexture(texture);

    ShaderProgram *sp = GetShaderProgram();
    String vshaderFilepath = xmlInfo->GetFilepath("VertexShader");
    String fshaderFilepath = xmlInfo->GetFilepath("FragmentShader");
    if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
        vshaderFilepath != sp->GetVertexShader()->GetFilepath() ||
        fshaderFilepath != sp->GetFragmentShader()->GetFilepath()
       )
    {
        SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));
    }
}

void Material::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Material");

    xmlInfo->SetColor("DiffuseColor", GetDiffuseColor());
    xmlInfo->SetFloat("Shininess", GetShininess());
    xmlInfo->SetBool("ReceivesLighting", ReceivesLighting());

    String textureFilepath = m_texture ? m_texture->GetFilepath() : "";
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
    xmlInfo->SetFilepath("VertexShader", vsFile, "vert");
    xmlInfo->SetFilepath("FragmentShader", fsFile, "frag");
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
        GetShaderProgram()->SetTexture("B_texture_0", m_texture);
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

void Material::OnRenderingStarts(GameObject *go, ShaderProgram *sp)
{
    sp->SetColor("B_material_diffuse_color", m_diffuseColor);
    sp->SetVec2("B_screen_size", Screen::GetSize());
    sp->SetFloat("B_material_shininess", m_shininess);
    sp->SetFloat("B_material_receivesLighting",
                                 m_receivesLighting ? 1.0f : 0.0f);

    float alphaCutoff = m_texture ? m_texture->GetAlphaCutoff() : -1.0f;
    sp->SetTexture("B_texture_0", m_texture);
    sp->SetFloat("B_alphaCutoff", alphaCutoff);
    sp->SetFloat("B_hasTexture", m_texture ? 1 : 0);
}
