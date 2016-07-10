#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Bang.h"

#include <list>
#include <vector>

#include "SingleLineRenderer.h"
#include "EditorGameObject.h"

class EditorFloor : public EditorGameObject
{
public:

    const float TileSize = 15.0f;
    const int GridSize = 30;     // Number of lines

    std::vector<SingleLineRenderer*> lineRenderers;

    EditorFloor();
    ~EditorFloor();

    void OnUpdate() override;
};

#endif // EDITORFLOOR_H
