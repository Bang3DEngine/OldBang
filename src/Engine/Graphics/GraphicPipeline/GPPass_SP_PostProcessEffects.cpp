#include "Bang/GPPass_SP_PostProcessEffects.h"

#include "Bang/Scene.h"
#include "Bang/AssetsManager.h"
#include "Bang/G_ShaderProgram.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/PostProcessEffect.h"

#ifdef BANG_EDITOR
#include "Bang/EditorState.h"
#endif

GPPass_SP_PostProcessEffects::GPPass_SP_PostProcessEffects(
        GraphicPipeline *graphicPipeline,
        PostProcessEffect::Type type,
        const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_type = type;
}

template<class T>
struct PointerComparison { bool operator()(T* a, T* b) { return *a < *b; } };

void GPPass_SP_PostProcessEffects::InPass(const List<Renderer *> &renderers,
                                          const List<GameObject *> &sceneChildren)
{
    #ifdef BANG_EDITOR
    if (!EditorState::IsShowingGameTab()) { return; }
    #endif

    List<PostProcessEffect*> postProcessEffects =
                       p_scene->GetComponentsInChildren<PostProcessEffect>();
    postProcessEffects.Sort( [](PostProcessEffect *a, PostProcessEffect *b)
                                   { return *a < *b; } );

    for (PostProcessEffect *postProcessEffect : postProcessEffects)
    {
        if (!postProcessEffect->IsEnabled() ||
             postProcessEffect->GetType() != m_type) { continue; }

        G_ShaderProgram *postProcessShaderProgram =
                         postProcessEffect->GetPostProcessShaderProgram();
        if (postProcessShaderProgram)
        {
            GL::SetTestDepth(false);
            p_gbuffer->Bind();
            p_gbuffer->SetStencilTest(false);
            p_gbuffer->SetStencilWrite(false);
            postProcessShaderProgram->Bind();
            p_gbuffer->ApplyPass(postProcessShaderProgram, true);
            postProcessShaderProgram->UnBind();
            p_gbuffer->UnBind();
            GL::SetTestDepth(true);
        }
    }
}
