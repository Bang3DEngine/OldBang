#include "Bang/Material.h"

#include "Bang/Paths.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/Vector4.h"
#include "Bang/Texture.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

Material::Material()
{
    RH<ShaderProgram> sp;
    Resources::Create<ShaderProgram>(&sp,
                                     EPATH("Shaders/G_Default.vert"),
                                     EPATH("Shaders/G_Default.frag"));
    SetShaderProgram(sp.Get());
}

Material::~Material()
{
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_uvMultiply = uvMultiply;
}

void Material::SetShaderProgram(ShaderProgram* program)
{
    p_shaderProgram.Set(program);
}

void Material::SetTexture(Texture2D* texture)
{
    p_texture.Set(texture);
    if (p_texture)
    {
        ShaderProgram *sp = GetShaderProgram();
        if (sp) { sp->Set("B_Texture0", p_texture.Get()); }
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

const Vector2 &Material::GetUvMultiply() const { return m_uvMultiply; }
ShaderProgram* Material::GetShaderProgram() const { return p_shaderProgram.Get(); }
Texture2D* Material::GetTexture() const { return p_texture.Get(); }
bool Material::IsReceivesLighting() const { return m_receivesLighting; }
float Material::GetShininess() const { return m_shininess; }
const Color& Material::GetDiffuseColor() const { return m_diffuseColor; }

void Material::Bind() const
{
    ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->Bind();

    sp->Set("B_UvMultiply",               GetUvMultiply());
    sp->Set("B_MaterialDiffuseColor",     GetDiffuseColor());
    sp->Set("B_MaterialShininess",        GetShininess());
    sp->Set("B_MaterialReceivesLighting", IsReceivesLighting());

    float alphaCutoff = GetTexture() ? GetTexture()->GetAlphaCutoff() : -1.0f;
    sp->Set("B_Texture0",  GetTexture());
    sp->Set("B_AlphaCutoff", alphaCutoff);
    sp->Set("B_HasTexture", (GetTexture() != nullptr));
}

void Material::UnBind() const
{
    // ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    // sp->UnBind();
}

void Material::CloneInto(ICloneable *clone) const
{
    Asset::CloneInto(clone);

    Material *matClone = Cast<Material*>(clone);

    matClone->SetShaderProgram(GetShaderProgram());
    matClone->SetDiffuseColor(GetDiffuseColor());
    matClone->SetReceivesLighting(IsReceivesLighting());
    matClone->SetShininess(GetShininess());
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
    {
        RH<Texture2D> texture;
        Resources::Load<Texture2D>(&texture, xml.Get<GUID>("Texture"));
        SetTexture(texture.Get());
    }

    RH<Shader> vShader;
    if (xml.Contains("VertexShader"))
    {  Resources::Load<Shader>(&vShader, xml.Get<GUID>("VertexShader"));  }

    RH<Shader> fShader;
    if (xml.Contains("FragmentShader"))
    { Resources::Load<Shader>(&fShader, xml.Get<GUID>("FragmentShader")); }

    if (vShader && fShader)
    {
        RH<ShaderProgram> newSp;
        Resources::Create<ShaderProgram>(&newSp, vShader.Get(), fShader.Get());
        SetShaderProgram(newSp.Get());
    }
}

void Material::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    xmlInfo->Set("DiffuseColor",     GetDiffuseColor());
    xmlInfo->Set("Shininess",        GetShininess());
    xmlInfo->Set("ReceivesLighting", IsReceivesLighting());
    xmlInfo->Set("UvMultiply",       GetUvMultiply());

    Texture2D* tex = GetTexture();
    xmlInfo->Set("Texture",  tex ? tex->GetGUID() : GUID::Empty());

    ShaderProgram *sp = GetShaderProgram();
    Shader* vShader = (sp ? sp->GetVertexShader()   : nullptr);
    Shader* fShader = (sp ? sp->GetFragmentShader() : nullptr);
    xmlInfo->Set("VertexShader",   vShader);
    xmlInfo->Set("FragmentShader", fShader);
}
