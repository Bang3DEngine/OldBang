#ifndef EDITORDEBUGGAMEOBJECT_H
#define EDITORDEBUGGAMEOBJECT_H

#include "List.h"

#include "Bang.h"

#include "Time.h"
#include "Color.h"
#include "Gizmos.h"
#include "EditorGameObject.h"

class EditorDebugGameObject : public EditorGameObject
{
private:
    struct DebugLine
    {
        Vector3 m_origin, m_destiny;
        Color m_color = Color::green;
        bool m_depthTest = false;
        float m_lineWidth = 0.0f;
        float m_elapsedTimeSecs = 0;
        float m_livingTimeSecs = 0;
        bool m_screen = false;

        DebugLine(const Vector3& origin, const Vector3 &destiny,
                  const Color &color, float lineWidth, float livingTimeSecs,
                  bool depthTest, bool screen) :
                    m_origin(origin), m_destiny(destiny), m_color(color),
                    m_depthTest(depthTest), m_lineWidth(lineWidth),
                    m_livingTimeSecs(livingTimeSecs), m_screen(screen)
        {
        }
    };

    List<DebugLine> m_debugLines;

    void DrawLines(bool depthEnabled);

public:

    EditorDebugGameObject();
    virtual ~EditorDebugGameObject();

    void DrawLine(const Vector3 &origin, const Vector3 &destiny,
                  const Color &color = Color::green,
                  float lineWidth = 1.0f, float livingTimeSecs = -1.0f,
                  bool depthTest = true);

    void DrawScreenLine(const Vector2 &origin, const Vector2 &destiny,
                        const Color &color = Color::green,
                        float lineWidth = 1.0f, float livingTimeSecs = -1.0f);

    void OnUpdate() override;
    void OnDrawGizmos() override;
    void OnDrawGizmosOverlay() override;
};

#endif // EDITORDEBUGGAMEOBJECT_H
