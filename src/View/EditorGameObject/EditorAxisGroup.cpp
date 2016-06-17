#include "EditorAxisGroup.h"

#include "WindowMain.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup() : EditorGameObject("EditorAxisGroup")
{
    axisX = new EditorTranslateAxis(EditorTranslateAxis::EditorTranslateAxisDirection::X);
    axisY = new EditorTranslateAxis(EditorTranslateAxis::EditorTranslateAxisDirection::Y);
    axisZ = new EditorTranslateAxis(EditorTranslateAxis::EditorTranslateAxisDirection::Z);

    AddChild(axisX);
    AddChild(axisY);
    AddChild(axisZ);

    AddComponent<Transform>();
}

EditorTranslateAxisGroup::~EditorTranslateAxisGroup()
{
}

void EditorTranslateAxisGroup::OnUpdate()
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

