#include "EditorAxis.h"

EditorAxis::EditorAxis() : EditorGameObject()
{
    SetName("EditorAxis");

    xAxisLine = AddComponent<LineRenderer>();
    yAxisLine = AddComponent<LineRenderer>();
    zAxisLine = AddComponent<LineRenderer>();

    matx = new Material(*xAxisLine->GetMaterial());
    maty = new Material(*yAxisLine->GetMaterial());
    matz = new Material(*zAxisLine->GetMaterial());

    matx->SetDiffuseColor(glm::vec4(1, 0, 0, 1));
    maty->SetDiffuseColor(glm::vec4(0, 1, 0, 1));
    matz->SetDiffuseColor(glm::vec4(0, 0, 1, 1));

    xAxisLine->SetMaterial(matx);
    yAxisLine->SetMaterial(maty);
    zAxisLine->SetMaterial(matz);

    float axisLength = 5.0f;
    xAxisLine->SetDestiny(glm::vec3(1,0,0) * axisLength);
    yAxisLine->SetDestiny(glm::vec3(0,1,0) * axisLength);
    zAxisLine->SetDestiny(glm::vec3(0,0,-1) * axisLength);

    transform = AddComponent<Transform>();
    transform->SetPosition(glm::vec3(0));

    this->SetRenderLayer(5);

    //The scale will be always the same, no matter the size of the gameObject
    transform->ignoreParentTransformMask =
            Transform::IgnoreParentTransformMask::IgnoreScale;
}

EditorAxis::~EditorAxis()
{
    delete matx;
    delete maty;
    delete matz;
}
