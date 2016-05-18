#ifndef EDITORDEBUGGAMEOBJECT_H
#define EDITORDEBUGGAMEOBJECT_H

#include "Bang.h"

#include <vector>

#include "LineRenderer.h"
#include "EditorGameObject.h"

class EditorDebugGameObject : public EditorGameObject
{
private:
    std::vector<LineRenderer*> lineRenderers;

public:

    EditorDebugGameObject();

    void DrawLine(const Vector3 &origin,
                  const Vector3 &destiny,
                  float lineWidth = 1.0f,
                  int msTime = -1,
                  bool depthTest = true);
};

#endif // EDITORDEBUGGAMEOBJECT_H
