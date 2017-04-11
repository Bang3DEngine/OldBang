#include "CircleCulledRenderer.h"

#include "Bang/Math.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"

CircleCulledRenderer::CircleCulledRenderer()
{

}

void CircleCulledRenderer::OnUpdate()
{
    LineRenderer::OnUpdate();
    RefreshPoints();
}

void CircleCulledRenderer::RefreshPoints()
{
    const float radius = GetRadius();
    const int segments = 64;

    Matrix4 localToWorld;
    transform->GetLocalToWorldMatrix(&localToWorld);
    const Vector3 center = transform->GetPosition();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    const Vector3 camPos = cam->transform->GetPosition();

    m_points.Clear();
    const float step = (2.0f * Math::PI) / segments;
    for (int i = 0;  i < segments; ++i)
    {
        Vector3 pLocal = GetCircleLocalPoint(step * i) * radius;
        Vector3 pWorld = (localToWorld * Vector4(pLocal, 1)).xyz();
        float dot = Vector3::Dot( (pWorld - center).Normalized(),
                                  (camPos - center).Normalized());
        if (dot >= -0.1)
        {
            Vector3 pLocalNext = GetCircleLocalPoint(step * (i+1)) * radius;
            m_points.PushBack(pLocal);
            m_points.PushBack(pLocalNext);
        }
    }

    LineRenderer::RefreshPoints();
}

void CircleCulledRenderer::GetTwoClosestPointsInScreenSpace(
        const Vector2 &sOrigin,
        Vector2 *p0, int *i0,
        Vector2 *p1, int *i1) const
{
    Matrix4 localToWorld;
    transform->GetLocalToWorldMatrix(&localToWorld);

    float d0, d1; d0 = d1 = 99999.9f;
    Vector3 latestPoint = m_points[0];
    for (int i = 0; i < m_points.Size() - 1; ++i)
    {
        Vector3 pLocal = m_points[i];
        if (latestPoint == pLocal) { continue; }

        Vector3 worldP = (localToWorld * Vector4(pLocal,1)).xyz();
        Camera *cam = SceneManager::GetActiveScene()->GetCamera();
        Vector2 sP = cam->WorldToScreenNDCPoint(worldP);

        float d = Vector2::Distance(sP, sOrigin);
        if (d < d0)
        {
            *p1 = *p0;
            *i1 = *i0;
            d1 = d0;

            *p0 = sP;
            *i0 = i;
            d0 = d;
        }
        else if (d < d1)
        {
            *p1 = sP;
            *i1 = i;
            d1 = d;
        }
        latestPoint = pLocal;
    }
}

Vector3 CircleCulledRenderer::GetCircleLocalPoint(float angle) const
{
    return Vector3(Math::Cos(angle), Math::Sin(angle), 0);
}

float CircleCulledRenderer::GetRadius() const
{
    return 0.5f;
}
