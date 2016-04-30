#include "EditorAxis.h"

EditorAxis::EditorAxis() : EditorEntity()
{
    SetName("EditorAxis");

    xAxisLine = AddPart<LineRenderer>();
    yAxisLine = AddPart<LineRenderer>();
    zAxisLine = AddPart<LineRenderer>();


    matx = new Material(*xAxisLine->GetMaterial());
    maty = new Material(*yAxisLine->GetMaterial());
    matz = new Material(*zAxisLine->GetMaterial());

    matx->SetDiffuseColor(glm::vec4(1, 0, 0, 1));
    maty->SetDiffuseColor(glm::vec4(0, 1, 0, 1));
    matz->SetDiffuseColor(glm::vec4(0, 0, 1, 1));

    xAxisLine->SetMaterial(matx);
    yAxisLine->SetMaterial(maty);
    zAxisLine->SetMaterial(matz);


    float axisLength = 9999.0f;
    xAxisLine->SetDestiny(glm::vec3(1,0,0) * axisLength);
    yAxisLine->SetDestiny(glm::vec3(0,1,0) * axisLength);
    zAxisLine->SetDestiny(glm::vec3(0,0,-1) * axisLength);

    transform = AddPart<Transform>();
    transform->SetPosition(glm::vec3(0));
}

EditorAxis::~EditorAxis()
{
    delete matx, maty, matz;
}
