#ifndef EDITORDEBUGGAMEOBJECT_H
#define EDITORDEBUGGAMEOBJECT_H

#include "Bang.h"

#include <vector>

#include "SingleLineRenderer.h"
#include "EditorGameObject.h"

class EditorDebugGameObject : public EditorGameObject
{
private:
    std::vector<SingleLineRenderer*> singleLineRenderers;

public:

    EditorDebugGameObject();

    void DrawLine(const Vector3 &origin,
                  const Vector3 &destiny,
                  float lineWidth = 1.0f,
                  int msTime = -1,
                  bool depthTest = true);
};

#endif // EDITORDEBUGGAMEOBJECT_H
