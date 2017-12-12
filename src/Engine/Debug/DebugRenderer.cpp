#include "Bang/DebugRenderer.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Gizmos.h"
#include "Bang/SceneManager.h"

USING_NAMESPACE_BANG

void DebugRenderer::RenderLine(const Vector3 &origin,
                               const Vector3 &end,
                               const Color &color,
                               float time,
                               float thickness,
                               bool depthTest)
{
    DebugRenderLine drl;
    drl.origin = origin;
    drl.end = end;
    drl.color = color;
    drl.destroyTimestamp = Time::GetNow_Seconds() + time;
    drl.thickness = thickness;
    drl.depthTest = depthTest;

    DebugRenderer *dr = DebugRenderer::GetActive();
    ASSERT(dr);
    dr->m_linesToRender.PushBack(drl);
}

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::Render(bool withDepth)
{
    GL::Function prevDepthFunc = GL::GetDepthFunc();
    for (auto it = m_linesToRender.Begin(); it != m_linesToRender.End(); )
    {
        const DebugRenderLine &drl = *it;
        if (drl.depthTest != withDepth)
        {
            ++it;
        }
        else if (Time::GetNow_Seconds() >= drl.destroyTimestamp)
        {
            it = m_linesToRender.Remove(it);
        }
        else
        {
            GL::SetDepthFunc(drl.depthTest ? GL::Function::LEqual :
                                             GL::Function::Always);
            Gizmos::SetLineWidth(drl.thickness);
            Gizmos::SetColor(drl.color);
            Gizmos::RenderLine(drl.origin, drl.end);
            ++it;
        }
    }
    GL::SetDepthFunc(prevDepthFunc);
}

DebugRenderer *DebugRenderer::GetActive()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->p_debugRenderer : nullptr;
}

