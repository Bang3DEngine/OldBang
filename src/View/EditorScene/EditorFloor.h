#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Array.h"

#include "GameObject.h"

class SingleLineRenderer;
class EditorFloor : public GameObject
{
public:

    const float c_tileSize = 15.0f;
    const int c_gridSize = 30;     // Number of lines

    Array<SingleLineRenderer*> m_lineRenderers;

    EditorFloor();
    virtual ~EditorFloor();

    void OnUpdate() override;
    virtual void OnDrawGizmos() override;

private:
    Material *m_material = nullptr;
};

#endif // EDITORFLOOR_H
