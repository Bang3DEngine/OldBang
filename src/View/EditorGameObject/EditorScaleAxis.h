#ifndef EDITORSCALEAXIS_H
#define EDITORSCALEAXIS_H


#include "EditorAxis.h"

class Renderer;
class GameObject;
class EditorGameObject;
class SingleLineRenderer;
class EditorScaleAxis : public EditorAxis
{
private:
    EditorGameObject *m_axisCap = nullptr;
    SingleLineRenderer *m_line = nullptr;

protected:

    virtual Renderer *GetAxisRenderer() const override;

public:
    EditorScaleAxis(EditorAxis::EditorAxisDirection dir, GameObject *attachedGameObject);
    virtual ~EditorScaleAxis();

    void OnUpdate() override;
    void OnDrawGizmosOverlay() override;
};


#endif // EDITORSCALEAXIS_H
