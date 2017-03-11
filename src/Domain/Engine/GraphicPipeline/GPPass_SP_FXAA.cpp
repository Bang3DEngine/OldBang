#include "GPPass_SP_FXAA.h"

#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"

GPPass_SP_FXAA::GPPass_SP_FXAA(GraphicPipeline *graphicPipeline,
                               const List<GPPass *> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_fxaaSP = new ShaderProgram(
                IO::ToAbsolute("Shaders/SP_ScreenPass.vert", true),
                IO::ToAbsolute("Shaders/SP_FXAA.frag", true));
}

void GPPass_SP_FXAA::InPass(const List<Renderer *> &renderers,
                            const List<GameObject *> &sceneChildren)
{
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->ApplyPass(m_fxaaSP, true);
}
