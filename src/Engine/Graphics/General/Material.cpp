#include "Bang/Material.h"

#include "Bang/Paths.h"
#include "Bang/Screen.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/Vector4.h"
#include "Bang/Texture.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

Material::Material()
{
    ShaderProgram *sp = new ShaderProgram();
    sp->Load(EPATH("Shaders/G_Default.vert_g"),
             EPATH("Shaders/G_Default.frag_g"));
    SetShaderProgram(sp);
}

Material::~Material()
{
}

void Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_uvMultiply = uvMultiply;
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
        if (sp) { sp->Set("B_Texture0", m_texture); }
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
    return m_shaderProgram;
}

const Texture2D *Material::GetTexture() const
{
    return m_texture;
}

bool Material::IsReceivesLighting() const
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

    sp->Set("B_ScreenSize", Vector2f(Screen::GetSizeS()));

    sp->Set("B_UvMultiply",              GetUvMultiply());
    sp->Set("B_MaterialDiffuseColor",     GetDiffuseColor());
    sp->Set("B_MaterialShininess",        GetShininess());
    sp->Set("B_MaterialReceivesLighting", IsReceivesLighting() ? 1.0f : 0.0f);

    float alphaCutoff = GetTexture() ? GetTexture()->GetAlphaCutoff() : -1.0f;
    sp->Set("B_Texture0",  GetTexture());
    sp->Set("B_AlphaCutoff", alphaCutoff);
    sp->Set("B_HasTexture",   GetTexture() != nullptr);

    GL::ApplyToShaderProgram(sp);
}

void Material::UnBind() const
{
    ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->UnBind();
}

void Material::CloneInto(ICloneable *clone) const
{
    Material *matClone = static_cast<Material*>(clone);

    matClone->SetShaderProgram(GetShaderProgram());
    matClone->SetDiffuseColor(GetDiffuseColor());
    matClone->SetReceivesLighting(IsReceivesLighting());
    matClone->SetShininess(GetShininess());
    matClone->SetTexture(GetTexture());

    ShaderProgram *sp = GetShaderProgram();
    if (sp && sp->GetVertexShader() && sp->GetFragmentShader())
    {
        Shader *vshader = sp->GetVertexShader();
        Shader *fshader = sp->GetFragmentShader();
        sp = new ShaderProgram();
        sp->Load(vshader->GetResourceFilepath(), fshader->GetResourceFilepath());
    }
    matClone->SetShaderProgram(sp);

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
    ShaderProgram *sp = GetShaderProgram();
    if (sp)
    {
        if (sp->GetVertexShader()) { vsPath = sp->GetVertexShader()->GetResourceFilepath(); }
        if (sp->GetFragmentShader()) { fsPath = sp->GetFragmentShader()->GetResourceFilepath(); }
    }
    xmlInfo->Set("VertexShader",   vsPath);
    xmlInfo->Set("FragmentShader", fsPath);
}
