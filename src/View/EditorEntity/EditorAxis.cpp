#include "EditorAxis.h"

EditorAxis::EditorAxis() : EditorEntity()
{
    SetName("EditorAxis");

    xAxisLine = AddPart<LineRenderer>();
    yAxisLine = AddPart<LineRenderer>();
    zAxisLine = AddPart<LineRenderer>();

    xAxisLine->GetMaterial()->SetDiffuseColor(glm::vec4(1, 0, 0, 1));
    yAxisLine->GetMaterial()->SetDiffuseColor(glm::vec4(0, 1, 0, 1));
    zAxisLine->GetMaterial()->SetDiffuseColor(glm::vec4(0, 0, 1, 1));

    float axisLength = 9999.0f;
    xAxisLine->SetDestiny(glm::vec3(1,0,0) * axisLength);
    yAxisLine->SetDestiny(glm::vec3(0,1,0) * axisLength);
    zAxisLine->SetDestiny(glm::vec3(0,0,1) * axisLength);

    transform = AddPart<Transform>();
    transform->SetPosition(glm::vec3(0));
}

EditorAxis::~EditorAxis()
{

}
