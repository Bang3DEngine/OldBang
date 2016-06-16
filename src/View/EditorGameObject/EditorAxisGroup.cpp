#include "EditorAxisGroup.h"

#include "WindowMain.h"

EditorAxisGroup::EditorAxisGroup() : EditorGameObject("EditorAxisGroup")
{
    axisX = new EditorAxis(EditorAxis::EditorAxisDirection::X);
    axisY = new EditorAxis(EditorAxis::EditorAxisDirection::Y);
    axisZ = new EditorAxis(EditorAxis::EditorAxisDirection::Z);

    AddChild(axisX);
    AddChild(axisY);
    AddChild(axisZ);

    AddComponent<Transform>();

    globalCoords = !WindowMain::GetInstance()->buttonGlobalCoords->
                    text().contains("lobal");
}

EditorAxisGroup::~EditorAxisGroup()
{
}

void EditorAxisGroup::OnUpdate()
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

            if(globalCoords)
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

void EditorAxisGroup::OnButtonGlobalCoordsClicked(bool globalCoords)
{
    this->globalCoords = globalCoords;
}

