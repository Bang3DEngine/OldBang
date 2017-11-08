#include "Bang/PostProcessEffect.h"

#include "Bang/Paths.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

PostProcessEffect::PostProcessEffect()
{
    m_shaderProgram = ShaderProgramFactory::GetEmptyProgram();

    Path vShaderPath = EPATH("Shaders/SP_ScreenPass.vert_pp");
    Shader *vShader = Resources::Load<Shader>(vShaderPath);
    m_shaderProgram->SetVertexShader(vShader);
}

PostProcessEffect::~PostProcessEffect()
{
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
        GBuffer *gbuffer = GEngine::GetInstance()->GetCurrentGBuffer();
        gbuffer->ApplyPass(m_shaderProgram, true);
        m_shaderProgram->UnBind();
    }
}

void PostProcessEffect::SetType(PostProcessEffect::Type type) { m_type = type; }
void PostProcessEffect::SetPriority(int priority) { m_priority = priority; }
void PostProcessEffect::SetPostProcessShader(Shader *postProcessShader)
{
    ENSURE(p_postProcessShader != postProcessShader);

    p_postProcessShader = postProcessShader;
    ENSURE(p_postProcessShader);

    m_shaderProgram->SetFragmentShader( p_postProcessShader );
}

PostProcessEffect::Type PostProcessEffect::GetType() const { return m_type; }
int PostProcessEffect::GetPriority() const { return m_priority; }
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
    return p_postProcessShader ? p_postProcessShader->GetResourceFilepath() : Path();
}

void PostProcessEffect::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    PostProcessEffect *ppe = SCAST<PostProcessEffect*>(clone);
    ppe->SetPostProcessShader( GetPostProcessShader() );
    ppe->SetType( GetType() );
    ppe->SetPriority( GetPriority() );
}

void PostProcessEffect::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Priority"))
    { SetPriority( xmlInfo.Get<int>("Priority") ); }

    if (xmlInfo.Contains("Type"))
    { SetType( xmlInfo.Get<Type>("Type") ); }

    if (xmlInfo.Contains("PostProcessShader"))
    {
        Path shaderFilepath = xmlInfo.Get<Path>("PostProcessShader");
        if (!p_postProcessShader ||
            shaderFilepath != p_postProcessShader->GetResourceFilepath())
        {
            Shader *postProcessShader = Resources::Load<Shader>(shaderFilepath);
            SetPostProcessShader(postProcessShader);
        }
    }
}

void PostProcessEffect::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("PostProcessShader", GetPostProcessShaderFilepath());
    xmlInfo->Set("Priority", GetPriority());
    xmlInfo->Set("Type", GetType());
}

bool operator<(const PostProcessEffect& lhs, const PostProcessEffect& rhs)
{
    return lhs.GetPriority() < rhs.GetPriority();
}
