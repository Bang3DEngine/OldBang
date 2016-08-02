#ifndef EDITORDEBUGGAMEOBJECT_H
#define EDITORDEBUGGAMEOBJECT_H

#include "Bang.h"

#include <list>

#include "SingleLineRenderer.h"
#include "EditorGameObject.h"

class EditorDebugGameObject : public EditorGameObject
{
private:
    struct DebugLine
    {
        SingleLineRenderer *slr = nullptr;
        float elapsedTimeSecs = 0;
        float livingTimeSecs = 0;

        DebugLine(SingleLineRenderer *slr, int livingTimeSecs) :
            slr(slr), livingTimeSecs(livingTimeSecs) {}
    };

    std::list<DebugLine> m_debugLines;

public:

    EditorDebugGameObject();
    virtual ~EditorDebugGameObject();

    void DrawLine(const Vector3 &origin,
                  const Vector3 &destiny,
                  const Vector3 &color = Vector3::zero,
                  float lineWidth = 1.0f,
                  float livingTimeSecs = -1.0f,
                  bool depthTest = true);

    void DrawLineScreen(
                  const Vector2 &origin,
                  const Vector2 &destiny,
                  const Vector3 &color = Vector3::zero,
                  float lineWidth = 1.0f,
                  float livingTimeSecs = -1.0f,
                  bool depthTest = true);

    void OnUpdate() override;
};

#endif // EDITORDEBUGGAMEOBJECT_H
