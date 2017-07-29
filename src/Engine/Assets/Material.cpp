#include "Bang/Material.h"

#include "Bang/Paths.h"
#include "Bang/G_Shader.h"
#include "Bang/Texture2D.h"
#include "Bang/G_Material.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"

Material::Material() : Asset()
{
    m_gMaterial = new G_Material();
    ShaderProgram *sp = new ShaderProgram();
    sp->Load(EPATH("Shaders/G_Default.vert_g"),
             EPATH("Shaders/G_Default.frag_g"));
    SetShaderProgram(sp);
}

Material::Material(const Material &m)
{
    m.CloneInto(this);
}

Material::~Material()
{
    if (m_gMaterial) { delete m_gMaterial; }
}

void Material::CloneInto(ICloneable *clone) const
{
    Material *matClone = static_cast<Material*>(clone);

    matClone->m_gMaterial = new G_Material(*m_gMaterial);

    ShaderProgram *sp = GetShaderProgram();
    if (sp && sp->GetVertexShader() && sp->GetFragmentShader())
    {
        G_Shader *vshader = sp->GetVertexShader();
        G_Shader *fshader = sp->GetFragmentShader();
        sp = new ShaderProgram();
        sp->Load(vshader->GetFilepath(), fshader->GetFilepath());
    }
    matClone->m_gMaterial->SetShaderProgram(sp);

    matClone->SetShaderProgram(sp);
    matClone->SetTexture(GetTexture());
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_gMaterial->SetUvMultiply(uvMultiply);
}

void Material::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    SetDiffuseColor(xmlInfo.Get<Color>("DiffuseColor"));
    SetShininess(xmlInfo.Get<float>("Shininess"));
    SetReceivesLighting(xmlInfo.Get<bool>("ReceivesLighting"));
    SetUvMultiply(xmlInfo.Get<Vector2>("UvMultiply"));

    Path texAssetFilepath = xmlInfo.Get<Path>("Texture");
    Texture2D *texture = AssetsManager::Load<Texture2D>(texAssetFilepath);
    SetTexture(texture);

    ShaderProgram *sp = GetShaderProgram();
    Path vshaderFilepath = xmlInfo.Get<Path>("VertexShader");
    Path fshaderFilepath = xmlInfo.Get<Path>("FragmentShader");
    if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
        vshaderFilepath != sp->GetVertexShader()->GetFilepath() ||
        fshaderFilepath != sp->GetFragmentShader()->GetFilepath()
       )
    {
        ShaderProgram *newSp = new ShaderProgram();
        newSp->Load(vshaderFilepath, fshaderFilepath);
        SetShaderProgram(newSp);
    }
}

void Material::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->Set("DiffuseColor",    GetDiffuseColor());
    xmlInfo->Set("Shininess",       GetShininess());
    xmlInfo->Set("ReceivesLighting", ReceivesLighting());
    xmlInfo->Set("UvMultiply",    GetUvMultiply());

    Path texFilepath = GetTexture() ? GetTexture()->GetFilepath() : Path::Empty;
    xmlInfo->Set("Texture", texFilepath);

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
    xmlInfo->Set("VertexShader", vsFile);
    xmlInfo->Set("FragmentShader", fsFile);
}

Material *Material::GetMissingMaterial()
{
    return AssetsManager::Load<Material>( EPATH("Materials/Missing.bmat") );
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    m_shaderProgram = program;
    m_gMaterial->SetShaderProgram(program);
}

void Material::SetTexture(const Texture2D *texture)
{
    m_texture = texture;
    m_gMaterial->SetTexture(texture);
}

void Material::SetReceivesLighting(bool receivesLighting)
{
    m_gMaterial->SetReceivesLighting(receivesLighting);
}

void Material::SetShininess(float shininess)
{
    m_gMaterial->SetShininess(shininess);
}

void Material::SetDiffuseColor(const Color &diffuseColor)
{
    m_gMaterial->SetDiffuseColor(diffuseColor);
}

ShaderProgram *Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

const Texture2D *Material::GetTexture() const
{
    return m_texture;
}

const Vector2 &Material::GetUvMultiply() const
{
    return m_gMaterial->GetUvMultiply();
}

bool Material::ReceivesLighting() const
{
    return m_gMaterial->ReceivesLighting();
}

float Material::GetShininess() const
{
    return m_gMaterial->GetShininess();
}

const Color& Material::GetDiffuseColor() const
{
    return m_gMaterial->GetDiffuseColor();
}

void Material::Bind() const
{
    m_gMaterial->Bind();
}

void Material::UnBind() const
{
    m_gMaterial->UnBind();
}
