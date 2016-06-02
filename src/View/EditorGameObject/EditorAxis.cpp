#include "EditorAxis.h"

EditorAxis::EditorAxis() : EditorGameObject("EditorAxis")
{
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

    float axisLength = 3.0f;
    xAxisLine->SetDestiny(Vector3(1,0,0) * axisLength);
    yAxisLine->SetDestiny(Vector3(0,1,0) * axisLength);
    zAxisLine->SetDestiny(Vector3(0,0,1) * axisLength);

    transform = AddComponent<Transform>();

    this->SetRenderLayer(5);
}

EditorAxis::~EditorAxis()
{
    delete matx;
    delete maty;
    delete matz;
}

void EditorAxis::OnStart()
{

}

void EditorAxis::OnUpdate()
{
    //Parent scale
    GameObject *parent = GetParent();
    if(parent != nullptr)
    {
        GameObject *selected = parent->GetParent();
        if(selected != nullptr)
        {
            Transform *st = selected->GetComponent<Transform>();
            if(st != nullptr)
            {
                Transform *t = GetComponent<Transform>();
                Box bbox = selected->GetObjectBoundingBox();
                t->SetPosition( bbox.GetCenter() );
                t->SetRotation( st->GetLocalRotation().Inversed() );
                t->SetScale(1.0f / st->GetScale());
            }
        }
    }
}




