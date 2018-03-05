#include "Bang/DebugRenderer.h"

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Gizmos.h"
#include "Bang/SceneManager.h"

USING_NAMESPACE_BANG

void DebugRenderer::Clear()
{
    DebugRenderer *dr = DebugRenderer::GetActive(); ASSERT(dr);
    dr->m_primitivesToRender.Clear();
}

void DebugRenderer::RenderLine(const Vector3 &origin,
                               const Vector3 &end,
                               const Color &color,
                               float time,
                               float thickness,
                               bool depthTest)
{
    CreateDebugRenderPrimitive(DebugRendererPrimitiveType::Line,
                               {origin, end},
                               color, time, thickness, depthTest);
}

void DebugRenderer::RenderPoint(const Vector3 &point,
                                const Color &color,
                                float time,
                                float thickness,
                                bool depthTest)
{
    CreateDebugRenderPrimitive(DebugRendererPrimitiveType::Point,
                               {point},
                               color, time, thickness, depthTest);
}

void DebugRenderer::RenderLineNDC(const Vector2 &originNDC,
                                  const Vector2 &endNDC,
                                  const Color &color,
                                  float time,
                                  float thickness,
                                  bool depthTest)
{
    CreateDebugRenderPrimitive(DebugRendererPrimitiveType::LineNDC,
                               {Vector3(originNDC,0), Vector3(endNDC,0)},
                               color, time, thickness, depthTest);
}

void DebugRenderer::RenderAABox(const AABox &aaBox,
                                const Color &color,
                                float time,
                                float thickness,
                                bool depthTest)
{
    const Vector3 ctr = aaBox.GetCenter();
    const Vector3 ext = aaBox.GetExtents();

    // Floor
    RenderLine(ctr + ext * Vector3(-1,-1,-1), ctr + ext * Vector3(-1,-1,1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(-1,-1,1), ctr + ext * Vector3(1,-1,1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,-1,1), ctr + ext * Vector3(1,-1,-1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,-1,-1), ctr + ext * Vector3(-1,-1,-1),
               color, time, thickness, depthTest);

    // Top
    RenderLine(ctr + ext * Vector3(-1,1,-1), ctr + ext * Vector3(-1,1,1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(-1,1,1), ctr + ext * Vector3(1,1,1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,1,1), ctr + ext * Vector3(1,1,-1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,1,-1), ctr + ext * Vector3(-1,1,-1),
               color, time, thickness, depthTest);

    // Columns (?) xd
    RenderLine(ctr + ext * Vector3(-1,-1,-1), ctr + ext * Vector3(-1,1,-1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(-1,-1,1), ctr + ext * Vector3(-1,1,1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,-1,-1), ctr + ext * Vector3(1,1,-1),
               color, time, thickness, depthTest);
    RenderLine(ctr + ext * Vector3(1,-1,1), ctr + ext * Vector3(1,1,1),
               color, time, thickness, depthTest);
}

void DebugRenderer::RenderAARectNDC(const AARect &rectNDC, const Color &color,
                                    float time, float thickness, bool depthTest)
{
    DebugRenderer::DebugRenderPrimitive *drp =
        CreateDebugRenderPrimitive(DebugRendererPrimitiveType::AARectNDC,
                                   {}, color, time, thickness, depthTest);
    if (drp)
    {
        drp->aaRectNDC = GL::FromViewportRectToViewportRectNDC(rectNDC);
    }
}

void DebugRenderer::RenderRectNDC(const Rect &rectNDC, const Color &color,
                                  float time, float thickness, bool depthTest)
{
    DebugRenderer::DebugRenderPrimitive *drp =
        CreateDebugRenderPrimitive(DebugRendererPrimitiveType::RectNDC,
                                   {}, color, time, thickness, depthTest);
    if (drp)
    {
        drp->rectNDCPoints = rectNDC.GetPoints();
    }
}

DebugRenderer::DebugRenderPrimitive*
DebugRenderer::CreateDebugRenderPrimitive(DebugRendererPrimitiveType primitive,
                                          const Array<Vector3> &points,
                                          const Color &color,
                                          float time,
                                          float thickness,
                                          bool depthTest)
{
    DebugRenderer *dr = DebugRenderer::GetActive();
    if (!dr) { Debug_Error("No active DebugRenderer!"); return nullptr; }

    DebugRenderPrimitive drp;
    drp.primitive = primitive;
    drp.origin = (points.Size() >= 1 ? points[0] : Vector3::Zero);
    drp.end =    (points.Size() >= 2 ? points[1] : Vector3::Zero);
    drp.color = color;
    drp.destroyTimestamp = Time::GetNow_Seconds() + time;
    drp.thickness = thickness;
    drp.depthTest = depthTest;
    drp.renderedOnce = false;

    dr->m_primitivesToRender.PushBack(drp);

    return &(dr->m_primitivesToRender.Back());
}

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::RenderPrimitives(bool withDepth)
{
    GL::Function prevDepthFunc = GL::GetDepthFunc();
    for (auto it = m_primitivesToRender.Begin(); it != m_primitivesToRender.End(); )
    {
        DebugRenderPrimitive *drp = &(*it);
        if (drp->depthTest != withDepth)
        {
            ++it;
        }
        else if (Time::GetNow_Seconds() >= drp->destroyTimestamp &&
                 drp->renderedOnce)
        {
            it = m_primitivesToRender.Remove(it);
        }
        else
        {
            Gizmos::Reset();
            GL::SetDepthFunc(drp->depthTest ? GL::Function::LEqual :
                                              GL::Function::Always);
            Gizmos::SetColor(drp->color);
            Gizmos::SetThickness(drp->thickness);
            switch (drp->primitive)
            {
                case DebugRendererPrimitiveType::Line:
                    Gizmos::RenderLine(drp->origin, drp->end);
                break;

                case DebugRendererPrimitiveType::LineNDC:
                    Gizmos::RenderViewportLineNDC(drp->origin.xy(), drp->end.xy());
                break;

                case DebugRendererPrimitiveType::Point:
                    Gizmos::RenderPoint(drp->origin);
                break;

                case DebugRendererPrimitiveType::AARectNDC:
                    Gizmos::RenderRectNDC(
                         GL::FromViewportRectNDCToViewportRect(drp->aaRectNDC) );
                break;

                case DebugRendererPrimitiveType::RectNDC:
                    Gizmos::RenderRectNDC(drp->rectNDCPoints);
                break;

                default: ASSERT(false); break;
            }
            drp->renderedOnce = true;
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

