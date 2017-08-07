#include "Bang/PostProcessEffect.h"

#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
#include "Bang/G_Shader.h"
#include "Bang/G_GBuffer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderManager.h"
#include "Bang/GraphicPipeline.h"

PostProcessEffect::PostProcessEffect()
{
    m_shaderProgram = new ShaderProgram();

    Path vShaderPath = EPATH("Shaders/SP_ScreenPass.vert_pp");
    G_Shader *vShader = ShaderManager::Load(G_Shader::Type::Vertex, vShaderPath);
    m_shaderProgram->SetVertexShader(vShader);
}

PostProcessEffect::~PostProcessEffect()
{
    if (m_shaderProgram) { delete m_shaderProgram; }
}

void PostProcessEffect::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);

    bool scenePostProcess = (GetType() == Type::AfterScene &&
                             renderPass == RenderPass::Scene_PostProcess);
    bool canvasPostProcess = (GetType() == Type::AfterCanvas &&
                              renderPass == RenderPass::Canvas_PostProcess);

    if (scenePostProcess || canvasPostProcess)
    {
        m_shaderProgram->Bind();
        G_GBuffer *gbuffer = GraphicPipeline::GetActive()->GetGBuffer();
        gbuffer->ApplyPass(m_shaderProgram, true);
        m_shaderProgram->UnBind();
    }
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

    m_shaderProgram->SetType(ShaderProgram::Type::PostProcess);
    m_shaderProgram->SetFragmentShader( p_postProcessShader );
}

void PostProcessEffect::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    PostProcessEffect *ppe = SCAST<PostProcessEffect*>(clone);
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

    Path shaderFilepath = xmlInfo.Get<Path>("PostProcessShader");
    if (!p_postProcessShader ||
        shaderFilepath != p_postProcessShader->GetFilepath())
    {
        G_Shader *postProcessShader =
                ShaderManager::Load(G_Shader::Type::Fragment, shaderFilepath);
        SetPostProcessShader(postProcessShader);
    }

    SetPriority( xmlInfo.Get<int>("Priority") );
    SetType( xmlInfo.Get<Type>("Type") );
}

void PostProcessEffect::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->Set("PostProcessShader", GetPostProcessShaderFilepath());
    xmlInfo->Set("Priority", GetPriority());
    xmlInfo->Set("Type", GetType());
}

bool operator<(const PostProcessEffect& lhs, const PostProcessEffect& rhs)
{
    return lhs.GetPriority() < rhs.GetPriority();
}
