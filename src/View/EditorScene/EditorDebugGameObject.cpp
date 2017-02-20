#include "EditorDebugGameObject.h"

#include "Time.h"
#include "Gizmos.h"

EditorDebugGameObject::EditorDebugGameObject() :
    GameObject("EditorDebugGameObject")
{
    AddHideFlag(HideFlags::HideAndDontSave);
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
            Gizmos::SetColor(dl.m_color);
            Gizmos::SetLineWidth(dl.m_lineWidth);
            Gizmos::SetReceivesLighting(false);

            if (dl.m_screen)
            {
                Gizmos::RenderScreenLine(dl.m_origin.xy(), dl.m_destiny.xy());
            }
            else
            {
                Gizmos::RenderLine(dl.m_origin, dl.m_destiny);
            }
        }
    }
}

void EditorDebugGameObject::DrawLine(const Vector3 &origin, const Vector3 &destiny,
                                     const Color &color, float lineWidth,
                                     float livingTimeSecs, bool depthTest)
{
    m_debugLines.PushBack(DebugLine(origin, destiny, color, lineWidth,
                                    livingTimeSecs, depthTest, false));
}

void EditorDebugGameObject::DrawScreenLine(const Vector2 &origin, const Vector2 &destiny,
                                           const Color &color, float lineWidth,
                                           float livingTimeSecs)
{
    m_debugLines.PushBack(DebugLine(Vector3(origin, 0), Vector3(destiny, 0),
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
            --it;
        }
    }
}

void EditorDebugGameObject::OnDrawGizmos()
{
    DrawLines(true);
}

void EditorDebugGameObject::OnDrawGizmosOverlay()
{
    DrawLines(false);
}



EditorDebugGameObject::DebugLine::DebugLine(const Vector3 &origin, const Vector3 &destiny, const Color &color, float lineWidth, float livingTimeSecs, bool depthTest, bool screen) :
    m_origin(origin), m_destiny(destiny), m_color(color),
    m_depthTest(depthTest), m_lineWidth(lineWidth),
    m_livingTimeSecs(livingTimeSecs), m_screen(screen)
{
}
