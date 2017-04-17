#ifndef EDITORSCALEAXISGROUP_H
#define EDITORSCALEAXISGROUP_H

#include "Bang/GameObject.h"

class Material;
class EditorScaleAxis;
/**
 * @brief The EditorScaleAxisGroup class represents a group of EditorScaleAxises.
 * There will be X,Y,Z EditorScaleAxis, which will be children of this GameObject.
 * We need to handle them separately to be able to detect the mouse events separately
 * on each one.
 */
class EditorScaleAxisGroup : public GameObject
{
private:
    EditorScaleAxis *m_axisX = nullptr;
    EditorScaleAxis *m_axisY = nullptr;
    EditorScaleAxis *m_axisZ = nullptr;
    EditorScaleAxis *m_axisCentral = nullptr; // Uniform scaling

public:
    EditorScaleAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorScaleAxisGroup();

    void OnEditorUpdate() override;
};

#endif // EDITORSCALEAXIS_H
