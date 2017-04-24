#include "GPPass_SP_PostProcessEffects.h"

#include "Bang/Scene.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/PostProcessEffect.h"

GPPass_SP_PostProcessEffects::GPPass_SP_PostProcessEffects(
        GraphicPipeline *graphicPipeline, const List<GPPass *> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
}

void GPPass_SP_PostProcessEffects::InPass(const List<Renderer *> &renderers,
                                          const List<GameObject *> &sceneChildren)
{
    List<PostProcessEffect*> postProcessEffects =
                       p_scene->GetComponentsInChildren<PostProcessEffect>();
    for (PostProcessEffect *postProcessEffect : postProcessEffects)
    {
        ShaderProgram *postProcessShaderProgram =
                         postProcessEffect->GetPostProcessShaderProgram();
        if (postProcessShaderProgram)
        {
            p_gbuffer->SetStencilTest(false);
            p_gbuffer->ApplyPass(postProcessShaderProgram, true);
        }
    }
}
