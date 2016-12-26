#ifndef EDITORAXISPLANE_H
#define EDITORAXISPLANE_H

#include "GameObject.h"

class Material;
class EditorAxis;
class MeshRenderer;
class EditorAxisPlane : public GameObject
{
private:
    EditorAxis *m_axis1 = nullptr;
    EditorAxis *m_axis2 = nullptr;
    MeshRenderer *m_planeRenderer = nullptr;
    Material *m_material = nullptr;

    void SetTransformAccordingToAxis();

public:
    EditorAxisPlane(EditorAxis *axis1, EditorAxis *axis2);
    virtual ~EditorAxisPlane();

    void OnUpdate() override;

};

#endif // EDITORAXISPLANE_H
