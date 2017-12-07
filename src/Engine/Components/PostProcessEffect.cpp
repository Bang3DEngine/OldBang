#include "Bang/PostProcessEffect.h"

#include "Bang/Paths.h"
#include "Bang/Shader.h"
#include "Bang/XMLNode.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

PostProcessEffect::PostProcessEffect()
{
    p_shaderProgram = Resources::Create<ShaderProgram>();

    Path vShaderPath = EPATH("Shaders/PP_ScreenPass.vert");
    RH<Shader> vShader = Resources::Load<Shader>(vShaderPath);
    p_shaderProgram.Get()->SetVertexShader(vShader.Get());
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
        p_shaderProgram.Get()->Bind();
        GEngine::GetActive()->ApplyScreenPass(p_shaderProgram.Get());
        p_shaderProgram.Get()->UnBind();
    }
}

void PostProcessEffect::SetType(PostProcessEffect::Type type) { m_type = type; }
void PostProcessEffect::SetPriority(int priority) { m_priority = priority; }
void PostProcessEffect::SetPostProcessShader(Shader *postProcessShader)
{
    ENSURE(p_postProcessShader.Get() != postProcessShader);

    p_postProcessShader.Set(postProcessShader);
    ENSURE(p_postProcessShader);

    p_shaderProgram.Get()->SetFragmentShader( GetPostProcessShader() );
}

PostProcessEffect::Type PostProcessEffect::GetType() const { return m_type; }
int PostProcessEffect::GetPriority() const { return m_priority; }
ShaderProgram* PostProcessEffect::GetPostProcessShaderProgram() const
{
    return p_shaderProgram.Get();
}
Shader* PostProcessEffect::GetPostProcessShader() const
{
    return p_postProcessShader.Get();
}
Path PostProcessEffect::GetPostProcessShaderFilepath() const
{
    return p_postProcessShader ?
                p_postProcessShader.Get()->GetResourceFilepath() : Path();
}

void PostProcessEffect::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    PostProcessEffect *ppe = Cast<PostProcessEffect*>(clone);
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
            shaderFilepath != p_postProcessShader.Get()->GetResourceFilepath())
        {
            RH<Shader> ppShader = Resources::Load<Shader>(shaderFilepath);
            SetPostProcessShader(ppShader.Get());
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
