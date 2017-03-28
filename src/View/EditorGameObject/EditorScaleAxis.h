#ifndef EDITORSCALEAXIS_H
#define EDITORSCALEAXIS_H


#include "Bang/EditorAxis.h"

class Renderer;
class GameObject;
class GameObject;
class SingleLineRenderer;
class EditorScaleAxis : public EditorAxis
{
public:
    EditorScaleAxis(EditorAxis::EditorAxisDirection dir, GameObject *attachedGameObject);
    virtual ~EditorScaleAxis();

    void OnUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;

protected:
    virtual Renderer *GetAxisRenderer() const override;

private:
    GameObject *m_axisCap = nullptr;
    SingleLineRenderer *m_line = nullptr;

     // If its the central axis, we must do little tweaks
    void ConfigureCentralAxis();
};


#endif // EDITORSCALEAXIS_H
