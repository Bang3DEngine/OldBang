#include "Bang/PostProcessEffect.h"

#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
#include "Bang/G_Shader.h"
#include "Bang/ShaderProgram.h"
#include "Bang/G_ShaderManager.h"

PostProcessEffect::PostProcessEffect()
{
    m_shaderProgram = new G_ShaderProgram();

    Path vShaderPath = EPATH("Shaders/SP_ScreenPass.vert_pp");
    G_Shader *vShader = G_ShaderManager::Load(G_Shader::Type::Vertex, vShaderPath);
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

void PostProcessEffect::SetPostProcessShader(G_Shader *postProcessShader)
{
    ENSURE(p_postProcessShader != postProcessShader);

    p_postProcessShader = postProcessShader;
    ENSURE(p_postProcessShader);

    m_shaderProgram->SetType(G_ShaderProgram::Type::PostProcess);
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

G_ShaderProgram *PostProcessEffect::GetPostProcessShaderProgram() const
{
    return m_shaderProgram;
}

G_Shader *PostProcessEffect::GetPostProcessShader() const
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
        G_Shader *postProcessShader =
                G_ShaderManager::Load(G_Shader::Type::Fragment, shaderFilepath);
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

bool operator<(const PostProcessEffect& lhs, const PostProcessEffect& rhs)
{
    return lhs.GetPriority() < rhs.GetPriority();
}
