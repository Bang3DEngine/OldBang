#include "GPPass_SP_PostProcessEffects.h"

#include "Bang/Scene.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/PostProcessEffect.h"

GPPass_SP_PostProcessEffects::GPPass_SP_PostProcessEffects(
        GraphicPipeline *graphicPipeline,
        PostProcessEffect::Type type,
        const List<GPPass *> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_type = type;
}

template<class T>
struct PointerComparison { bool operator()(T* a, T* b) { return *a < *b; } };

void GPPass_SP_PostProcessEffects::InPass(const List<Renderer *> &renderers,
                                          const List<GameObject *> &sceneChildren)
{
    List<PostProcessEffect*> postProcessEffects =
                       p_scene->GetComponentsInChildren<PostProcessEffect>();
    postProcessEffects.Sort( [](PostProcessEffect *a, PostProcessEffect *b)
                                   { return *a < *b; } );

    for (PostProcessEffect *postProcessEffect : postProcessEffects)
    {
        if (!postProcessEffect->IsEnabled() ||
             postProcessEffect->GetType() != m_type) { continue; }

        ShaderProgram *postProcessShaderProgram =
                         postProcessEffect->GetPostProcessShaderProgram();
        if (postProcessShaderProgram)
        {
            GL::SetTestDepth(false);
            p_gbuffer->Bind();
            p_gbuffer->SetStencilTest(false);
            p_gbuffer->SetStencilWrite(false);
            p_gbuffer->ApplyPass(postProcessShaderProgram, true);
            p_gbuffer->UnBind();
            GL::SetTestDepth(true);
        }
    }
}