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
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

Material::Material()
{
    SetShaderProgram( ShaderProgramFactory::GetDefault() );
}

Material::~Material()
{
}

void Material::SetUvOffset(const Vector2 &uvOffset)
{
    if (uvOffset != GetUvOffset())
    {
        m_uvOffset = uvOffset;
        PropagateMaterialChanged();
    }
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    if (uvMultiply != GetUvMultiply())
    {
        m_uvMultiply = uvMultiply;
        PropagateMaterialChanged();
    }
}

void Material::SetShaderProgram(ShaderProgram* program)
{
    if (p_shaderProgram.Get() != program)
    {
        p_shaderProgram.Set(program);
        PropagateMaterialChanged();
    }
}

void Material::SetTexture(Texture2D* texture)
{
    if (p_texture.Get() != texture)
    {
        if (GetTexture())
        {
            GetTexture()->EventEmitter<ITextureChangedListener>::
                          UnRegisterListener(this);
        }

        p_texture.Set(texture);
        ShaderProgram *sp = GetShaderProgram();
        if (sp) { sp->Set("B_Texture0", GetTexture()); }

        if (GetTexture())
        {
            GetTexture()->EventEmitter<ITextureChangedListener>::
                          RegisterListener(this);
        }
    }
}

void Material::SetReceivesLighting(bool receivesLighting)
{
    if (receivesLighting != GetReceivesLighting())
    {
        m_receivesLighting = receivesLighting;
        PropagateMaterialChanged();
    }
}

void Material::SetShininess(float shininess)
{
    if (shininess != GetShininess())
    {
        m_shininess = shininess;
        PropagateMaterialChanged();
    }
}

void Material::SetDiffuseColor(const Color &diffuseColor)
{
    if (diffuseColor != GetDiffuseColor())
    {
        m_diffuseColor = diffuseColor;
        PropagateMaterialChanged();
    }
}

const Vector2 &Material::GetUvOffset() const { return m_uvOffset; }
const Vector2 &Material::GetUvMultiply() const { return m_uvMultiply; }
ShaderProgram* Material::GetShaderProgram() const { return p_shaderProgram.Get(); }
Texture2D* Material::GetTexture() const { return p_texture.Get(); }
bool Material::GetReceivesLighting() const { return m_receivesLighting; }
float Material::GetShininess() const { return m_shininess; }
const Color& Material::GetDiffuseColor() const { return m_diffuseColor; }

void Material::Bind() const
{
    ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->Bind();

    sp->Set("B_UvOffset",                 GetUvOffset());
    sp->Set("B_UvMultiply",               GetUvMultiply());
    sp->Set("B_MaterialDiffuseColor",     GetDiffuseColor());
    sp->Set("B_MaterialShininess",        GetShininess());
    sp->Set("B_MaterialReceivesLighting", GetReceivesLighting());

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
    matClone->SetReceivesLighting(GetReceivesLighting());
    matClone->SetShininess(GetShininess());
    matClone->SetTexture(GetTexture());
}

void Material::OnTextureChanged(const Texture*)
{
    PropagateMaterialChanged();
}

void Material::PropagateMaterialChanged()
{
    EventEmitter<IMaterialChangedListener>::PropagateToListeners(
                &IMaterialChangedListener::OnMaterialChanged, this);
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

    if (xml.Contains("UvOffset"))
    { SetUvMultiply(xml.Get<Vector2>("UvOffset")); }

    if (xml.Contains("UvMultiply"))
    { SetUvMultiply(xml.Get<Vector2>("UvMultiply")); }

    if (xml.Contains("Texture"))
    { SetTexture(Resources::Load<Texture2D>(xml.Get<GUID>("Texture")).Get()); }

    RH<Shader> vShader;
    if (xml.Contains("VertexShader"))
    {  vShader = Resources::Load<Shader>(xml.Get<GUID>("VertexShader"));  }

    RH<Shader> fShader;
    if (xml.Contains("FragmentShader"))
    { fShader = Resources::Load<Shader>(xml.Get<GUID>("FragmentShader")); }

    if (vShader && fShader)
    {
        ShaderProgram *newSp =
                ShaderProgramFactory::Get(vShader.Get()->GetResourceFilepath(),
                                          fShader.Get()->GetResourceFilepath());
        SetShaderProgram(newSp);
    }
}

void Material::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    xmlInfo->Set("DiffuseColor",     GetDiffuseColor());
    xmlInfo->Set("Shininess",        GetShininess());
    xmlInfo->Set("ReceivesLighting", GetReceivesLighting());
    xmlInfo->Set("UvMultiply",       GetUvMultiply());
    xmlInfo->Set("UvOffset",         GetUvOffset());

    Texture2D* tex = GetTexture();
    xmlInfo->Set("Texture",  tex ? tex->GetGUID() : GUID::Empty());

    ShaderProgram *sp = GetShaderProgram();
    Shader* vShader = (sp ? sp->GetVertexShader()   : nullptr);
    Shader* fShader = (sp ? sp->GetFragmentShader() : nullptr);
    xmlInfo->Set("VertexShader",   vShader->GetGUID());
    xmlInfo->Set("FragmentShader", fShader->GetGUID());
}
