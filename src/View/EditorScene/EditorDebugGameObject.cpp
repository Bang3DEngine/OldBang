#include "EditorDebugGameObject.h"

EditorDebugGameObject::EditorDebugGameObject() :
    EditorGameObject("EditorDebugGameObject")
{
}

EditorDebugGameObject::~EditorDebugGameObject()
{
}

void EditorDebugGameObject::DrawLines(bool depthEnabled)
{
    for (auto it = m_debugLines.Begin(); it != m_debugLines.End(); ++it)
    {
        const DebugLine &dl = *it;
        bool draw = !(depthEnabled ^ dl.m_depthTest);
        if (draw)
        {
            if (dl.m_screen)
            {
                Gizmos::SetIgnoreMatrices(true, true, true);
            }
            else
            {
                Gizmos::SetIgnoreMatrices(false, false, false);
            }

            Gizmos::SetColor(dl.m_color);
            Gizmos::SetLineWidth(dl.m_lineWidth);
            Gizmos::SetReceivesLighting(false);

            Gizmos::RenderLine(dl.m_origin, dl.m_destiny);
        }
    }
}

void EditorDebugGameObject::DrawLine(const Vector3 &origin, const Vector3 &destiny,
                                     const Color &color, float lineWidth,
                                     float livingTimeSecs, bool depthTest)
{
    m_debugLines.push_back(DebugLine(origin, destiny, color, lineWidth,
                                     livingTimeSecs, depthTest, false));
}

void EditorDebugGameObject::DrawScreenLine(const Vector2 &origin, const Vector2 &destiny,
                                           const Color &color, float lineWidth,
                                           float livingTimeSecs)
{
    m_debugLines.push_back(DebugLine(Vector3(origin, 0), Vector3(destiny, 0),
                                     color, lineWidth,
                                     livingTimeSecs, false, true));
}

void EditorDebugGameObject::OnUpdate()
{
    // Remove the ones that have exhausted its time
    float dTime = Time::deltaTime;
    for (auto it = m_debugLines.Begin(); it != m_debugLines.End(); ++it)
    {
        DebugLine &dl = *it;
        dl.m_elapsedTimeSecs += dTime;
        if (dl.m_elapsedTimeSecs >= dl.m_livingTimeSecs)
        {
            it = m_debugLines.Remove(it);
        }
    }
}

void EditorDebugGameObject::OnDrawGizmos()
{
    Gizmos::Begin();
    DrawLines(true);
}

void EditorDebugGameObject::OnDrawGizmosNoDepth()
{
    Gizmos::Begin();
    DrawLines(false);
}

