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
bool Material::IsReceivesLighting() const { return m_gMaterial->ReceivesLighting(); }
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
        sp->Load(vshader->GetResourceFilepath(), fshader->GetResourceFilepath());
    }
    matClone->m_gMaterial->SetShaderProgram(sp);

    matClone->SetShaderProgram(sp);
    matClone->SetTexture(GetTexture());
}

void Material::Import(const Path &materialFilepath)
{
    ImportXMLFromFile(materialFilepath);
}

void Material::ImportXML(const XMLNode &xml)
{
    Asset::ImportXML(xml);

    if (xml.Contains("DiffuseColor"))
    { SetDiffuseColor(xml.Get<Color>("DiffuseColor")); }

    if (xml.Contains("Shininess"))
    { SetShininess(xml.Get<float>("Shininess")); }

    if (xml.Contains("ReceivesLighting"))
    { SetReceivesLighting(xml.Get<bool>("ReceivesLighting")); }

    if (xml.Contains("UvMultiply"))
    { SetUvMultiply(xml.Get<Vector2>("UvMultiply")); }

    if (xml.Contains("Texture"))
    { SetTexture( Resources::Load<Texture2D>( xml.Get<GUID>("Texture") ) ); }

    if (xml.Contains("VertexShader") && xml.Contains("FragmentShader"))
    {
        ShaderProgram *sp = GetShaderProgram();
        Path vsPath = xml.Get<Path>("VertexShader");
        Path fsPath = xml.Get<Path>("FragmentShader");
        if (!sp || !sp->GetVertexShader() || !sp->GetFragmentShader() ||
            vsPath != sp->GetVertexShader()->GetResourceFilepath()  ||
            fsPath != sp->GetFragmentShader()->GetResourceFilepath()
           )
        {
            ShaderProgram *newSp = new ShaderProgram();
            newSp->Load(vsPath, fsPath);
            SetShaderProgram(newSp);
        }
    }
}

void Material::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    xmlInfo->Set("DiffuseColor",     GetDiffuseColor());
    xmlInfo->Set("Shininess",        GetShininess());
    xmlInfo->Set("ReceivesLighting", IsReceivesLighting());
    xmlInfo->Set("UvMultiply",       GetUvMultiply());

    const Texture2D *tex = GetTexture();
    xmlInfo->Set("Texture",  tex ? tex->GetGUID() : GUID::Empty());

    Path vsPath, fsPath;
    G_ShaderProgram *sp = GetShaderProgram();
    if (sp)
    {
        if (sp->GetVertexShader()) { vsPath = sp->GetVertexShader()->GetResourceFilepath(); }
        if (sp->GetFragmentShader()) { fsPath = sp->GetFragmentShader()->GetResourceFilepath(); }
    }
    xmlInfo->Set("VertexShader",   vsPath);
    xmlInfo->Set("FragmentShader", fsPath);
}

