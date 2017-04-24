#include "PostProcessEffect.h"

#include "Bang/IO.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/ShaderManager.h"
#include "Bang/ShaderProgram.h"

PostProcessEffect::PostProcessEffect()
{
    m_shaderProgram = new ShaderProgram();

    String vShaderPath = IO::ToAbsolute("Shaders/SP_ScreenPass.vert", true);
    Shader *vShader = ShaderManager::Load(Shader::Type::Fragment, vShaderPath);
    m_shaderProgram->SetVertexShader(vShader);
}

PostProcessEffect::~PostProcessEffect()
{
    if (m_shaderProgram) { delete m_shaderProgram; }
}

void PostProcessEffect::SetPostProcessShader(Shader *postProcessShader)
{
    ENSURE(p_postProcessShader != postProcessShader);

    p_postProcessShader = postProcessShader;
    ENSURE(p_postProcessShader);

    m_shaderProgram->SetFragmentShader( p_postProcessShader );
}

ShaderProgram *PostProcessEffect::GetPostProcessShaderProgram() const
{
    return m_shaderProgram;
}

Shader *PostProcessEffect::GetPostProcessShader() const
{
    return p_postProcessShader;
}

String PostProcessEffect::GetPostProcessShaderFilepath() const
{
    return p_postProcessShader ? p_postProcessShader->GetFilepath() : "";
}

void PostProcessEffect::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    String shaderFilepath = xmlInfo.GetFilepath("PostProcessShader");
    if (!p_postProcessShader ||
        shaderFilepath != p_postProcessShader->GetFilepath())
    {
        Shader *postProcessShader = ShaderManager::Load(Shader::Type::Fragment,
                                                        shaderFilepath);
        SetPostProcessShader(postProcessShader);
    }
}

void PostProcessEffect::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->SetFilepath("PostProcessShader", GetPostProcessShaderFilepath(),
                         "frag glsl");
}
