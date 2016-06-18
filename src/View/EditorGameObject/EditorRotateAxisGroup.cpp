#include "EditorRotateAxisGroup.h"

EditorRotateAxisGroup::EditorRotateAxisGroup()
{
    axisX = new EditorRotateAxis(EditorRotateAxis::EditorRotateAxisDirection::X);
    axisY = new EditorRotateAxis(EditorRotateAxis::EditorRotateAxisDirection::Y);
    axisZ = new EditorRotateAxis(EditorRotateAxis::EditorRotateAxisDirection::Z);

    this->AddChild(axisX);
    this->AddChild(axisY);
    this->AddChild(axisZ);

    AddComponent<Transform>();
}

void EditorRotateAxisGroup::OnUpdate()
{
    //Attached GameObject positioning and scaling
    GameObject *attGameObject = GetAttachedGameObject();
    if(attGameObject != nullptr)
    {
        Transform *st = attGameObject->GetComponent<Transform>();
        if(st != nullptr)
        {
            Transform *t = GetComponent<Transform>();
            Box bbox = attGameObject->GetObjectBoundingBox();

            t->SetPosition( bbox.GetCenter() );

            if(Toolbar::GetInstance()->GetGlobalCoordsMode())
            {
                t->SetRotation( st->GetLocalRotation().Inversed() );
            }
            else
            {
                t->SetRotation( Quaternion() );
            }

            t->SetScale(1.0f / st->GetScale());
        }
    }
}
