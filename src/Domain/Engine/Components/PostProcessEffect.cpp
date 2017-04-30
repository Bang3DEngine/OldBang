#include "PostProcessEffect.h"

#include "Bang/IO.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/ShaderManager.h"
#include "Bang/ShaderProgram.h"

PostProcessEffect::PostProcessEffect()
{
    m_shaderProgram = new ShaderProgram();

    Path vShaderPath = EPATH("Shaders/SP_ScreenPass.vert_pp");
    Shader *vShader = ShaderManager::Load(Shader::Type::Vertex, vShaderPath);
    m_shaderProgram->SetVertexShader(vShader);
}

PostProcessEffect::~PostProcessEffect()
{
    if (m_shaderProgram) { delete m_shaderProgram; }
}

void PostProcessEffect::SetType(PostProcessEffect::Type type)
{
    m_type = type;
}

void PostProcessEffect::SetPriority(int priority)
{
    m_priority = priority;
}

void PostProcessEffect::SetPostProcessShader(Shader *postProcessShader)
{
    ENSURE(p_postProcessShader != postProcessShader);

    p_postProcessShader = postProcessShader;
    ENSURE(p_postProcessShader);

    m_shaderProgram->SetFragmentShader( p_postProcessShader );
}

void PostProcessEffect::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    PostProcessEffect *ppe = Object::SCast<PostProcessEffect>(clone);
    ppe->SetPostProcessShader( GetPostProcessShader() );
    ppe->SetType( GetType() );
    ppe->SetPriority( GetPriority() );
}

PostProcessEffect::Type PostProcessEffect::GetType() const
{
    return m_type;
}

int PostProcessEffect::GetPriority() const
{
    return m_priority;
}

ShaderProgram *PostProcessEffect::GetPostProcessShaderProgram() const
{
    return m_shaderProgram;
}

Shader *PostProcessEffect::GetPostProcessShader() const
{
    return p_postProcessShader;
}

Path PostProcessEffect::GetPostProcessShaderFilepath() const
{
    return p_postProcessShader ? p_postProcessShader->GetFilepath() : Path();
}

void PostProcessEffect::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    Path shaderFilepath = xmlInfo.GetFilepath("PostProcessShader");
    if (!p_postProcessShader ||
        shaderFilepath != p_postProcessShader->GetFilepath())
    {
        Shader *postProcessShader =
                ShaderManager::Load(Shader::Type::Fragment, shaderFilepath);
        SetPostProcessShader(postProcessShader);
    }

    SetPriority( xmlInfo.GetInt("Priority") );

    String typeStr = xmlInfo.GetEnumSelectedName("Type");
    if      (typeStr.Contains("Scene")) { SetType(Type::AfterScene); }
    else if (typeStr.Contains("Canvas")) { SetType(Type::AfterCanvas); }
}

void PostProcessEffect::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->SetFilepath("PostProcessShader", GetPostProcessShaderFilepath(),
                         "frag_pp");

    xmlInfo->SetInt("Priority", GetPriority());
    xmlInfo->SetEnum("Type", {"AfterScene", "AfterCanvas"}, int( GetType() ));
}

bool PostProcessEffect::operator<(const PostProcessEffect &ppe) const
{
    return GetPriority() < ppe.GetPriority();
}
