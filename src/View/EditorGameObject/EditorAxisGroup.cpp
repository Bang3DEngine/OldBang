#include "EditorAxisGroup.h"

EditorAxisGroup::EditorAxisGroup(GameObject *attachedGameObject) : EditorGameObject("EditorAxisGroup")
{
    this->p_attachedGameObject = attachedGameObject;
    p_etag = new EditorTranslateAxisGroup(attachedGameObject);
    p_erag = new EditorRotateAxisGroup(attachedGameObject);
    p_esag = new EditorScaleAxisGroup(attachedGameObject);

    AddChild(p_etag);
    AddChild(p_erag);
    AddChild(p_esag);

    DisableAllGroups();
}

void EditorAxisGroup::DisableAllGroups()
{
    p_etag->SetEnabled(false);
    p_erag->SetEnabled(false);
    p_esag->SetEnabled(false);
}

void EditorAxisGroup::OnUpdate()
{
    Toolbar::TransformMode tm = Toolbar::GetInstance()->
                                GetSelectedTransformMode();

    if (tm == Toolbar::TransformMode::Translate)
    {
        DisableAllGroups();
        p_etag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Rotate)
    {
        DisableAllGroups();
        p_erag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Scale)
    {
        DisableAllGroups();
        p_esag->SetEnabled(true);
    }

    //Attached GameObject positioning
    if (p_attachedGameObject)
    {
        Transform *at = p_attachedGameObject->transform;
        if (at)
        {
            transform->SetPosition(at->GetPosition()); // Pivot
            Box bbox = p_attachedGameObject->GetBoundingBox();
            transform->SetPosition( bbox.GetCenter() ); // Center

            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                transform->SetLocalRotation( Quaternion() );
            }
            else
            {
                transform->SetLocalRotation( at->GetRotation() );
            }

            Camera *cam = Canvas::GetCurrentScene()->GetCamera();
            Vector3 camPos = cam->gameObject->transform->GetPosition();
            Vector3 attPos = at->GetPosition();
            float distanceToCamera = Vector3::Distance(camPos, attPos);

            transform->SetScale(distanceToCamera * c_sizeBoost);
        }
    }
}


