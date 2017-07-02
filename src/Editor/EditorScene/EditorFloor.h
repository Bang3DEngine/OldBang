#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Bang/Array.h"
#include "Bang/GameObject.h"

class LineRenderer;
class EditorFloor : public GameObject
{
public:

    const float c_tileSize = 15.0f;
    const int c_gridSize = 30;     // Number of lines

    LineRenderer* m_lineRenderer = nullptr;

    EditorFloor();
    virtual ~EditorFloor();

    void OnEditorUpdate() override;
    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

private:
    Material *m_material = nullptr;
};

#endif // EDITORFLOOR_H
