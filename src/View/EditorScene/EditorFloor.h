#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Bang.h"

#include "List.h"
#include "Array.h"

#include "SingleLineRenderer.h"
#include "EditorGameObject.h"

class EditorFloor : public EditorGameObject
{
public:

    const float c_tileSize = 15.0f;
    const int c_gridSize = 30;     // Number of lines

    Array<SingleLineRenderer*> m_lineRenderers;

    EditorFloor();
    virtual ~EditorFloor();

    void OnUpdate() override;
};

#endif // EDITORFLOOR_H
