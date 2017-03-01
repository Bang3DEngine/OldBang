#ifndef EDITORDEBUGGAMEOBJECT_H
#define EDITORDEBUGGAMEOBJECT_H

#include "List.h"

#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameObject.h"

class EditorDebugGameObject : public GameObject
{
private:
    struct DebugLine
    {
        Vector3 m_origin, m_destiny;
        Color m_color = Color::Green;
        bool m_depthTest = false;
        float m_lineWidth = 0.0f;
        float m_elapsedTimeSecs = 0;
        float m_livingTimeSecs = 0;
        bool m_screen = false;

        DebugLine(const Vector3& origin, const Vector3 &destiny,
                  const Color &color, float lineWidth, float livingTimeSecs,
                  bool depthTest, bool screen);
    };

    List<DebugLine> m_debugLines;

    void DrawLines(bool depthEnabled);

public:

    EditorDebugGameObject();
    virtual ~EditorDebugGameObject();

    void DrawLine(const Vector3 &origin, const Vector3 &destiny,
                  const Color &color = Color::Green,
                  float lineWidth = 1.0f, float livingTimeSecs = -1.0f,
                  bool depthTest = true);

    void DrawScreenLine(const Vector2 &origin, const Vector2 &destiny,
                        const Color &color = Color::Green,
                        float lineWidth = 1.0f, float livingTimeSecs = -1.0f);

    void OnUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;
};

#endif // EDITORDEBUGGAMEOBJECT_H
