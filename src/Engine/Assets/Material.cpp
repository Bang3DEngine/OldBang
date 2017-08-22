#include "Bang/Material.h"

#include "Bang/Paths.h"
#include "Bang/G_Shader.h"
#include "Bang/Texture2D.h"
#include "Bang/G_Material.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"

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

void Material::Bind() const
{
    m_gMaterial->Bind();
}

void Material::UnBind() const
{
    m_gMaterial->UnBind();
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_gMaterial->SetUvMultiply(uvMultiply);
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

ShaderProgram *Material::GetShaderProgram() const { return m_shaderProgram; }
const Texture2D *Material::GetTexture() const { return m_texture; }
const Vector2& Material::GetUvMultiply() const { return m_gMaterial->GetUvMultiply(); }
bool Material::GetReceivesLighting() const { return m_gMaterial->ReceivesLighting(); }
 float Material::GetShininess() const { return m_gMaterial->GetShininess(); }
 const Color& Material::GetDiffuseColor() const { return m_gMaterial->GetDiffuseColor(); }

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

void Material::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    SetDiffuseColor(xmlInfo.Get<Color>("DiffuseColor"));
    SetShininess(xmlInfo.Get<float>("Shininess"));
    SetReceivesLighting(xmlInfo.Get<bool>("ReceivesLighting"));
    SetUvMultiply(xmlInfo.Get<Vector2>("UvMultiply"));
    SetTexture( Resources::Load<Texture2D>( xmlInfo.Get<GUID>("Texture") ) );

    ShaderProgram *sp = GetShaderProgram();
    Path vsPath = xmlInfo.Get<Path>("VertexShader");
    Path fsPath = xmlInfo.Get<Path>("FragmentShader");
    if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
        vsPath != sp->GetVertexShader()->GetFilepath()  ||
        fsPath != sp->GetFragmentShader()->GetFilepath()
       )
    {
        ShaderProgram *newSp = new ShaderProgram();
        newSp->Load(vsPath, fsPath);
        SetShaderProgram(newSp);
    }
}

void Material::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->Set("DiffuseColor",     GetDiffuseColor());
    xmlInfo->Set("Shininess",        GetShininess());
    xmlInfo->Set("ReceivesLighting", GetReceivesLighting());
    xmlInfo->Set("UvMultiply",       GetUvMultiply());
    xmlInfo->Set("Texture", GetTexture()->GetGUID());

    Path vsPath, fsPath;
    G_ShaderProgram *sp = GetShaderProgram();
    if (sp)
    {
        if (sp->GetVertexShader()) { vsPath = sp->GetVertexShader()->GetFilepath(); }
        if (sp->GetFragmentShader()) { fsPath = sp->GetFragmentShader()->GetFilepath(); }
    }
    xmlInfo->Set("VertexShader",   vsPath);
    xmlInfo->Set("FragmentShader", fsPath);
}

