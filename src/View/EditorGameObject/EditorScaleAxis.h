#ifndef EDITORSCALEAXIS_H
#define EDITORSCALEAXIS_H

#include "Bang.h"

#include "Matrix4.h"
#include "Material.h"
#include "Transform.h"
#include "EditorAxis.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

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
};


#endif // EDITORSCALEAXIS_H
