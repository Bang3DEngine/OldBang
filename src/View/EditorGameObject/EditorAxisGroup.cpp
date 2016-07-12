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

    AddComponent<Transform>();
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
        Transform *at = p_attachedGameObject->GetComponent<Transform>();
        if (at)
        {

            Transform *t = GetComponent<Transform>();

            t->SetPosition(at->GetPosition()); // Pivot
            Box bbox = p_attachedGameObject->GetBoundingBox();
            t->SetPosition( bbox.GetCenter() ); // Center

            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                t->SetLocalRotation( Quaternion() );
            }
            else
            {
                t->SetLocalRotation( at->GetRotation() );
            }

            Camera *cam = Canvas::GetCurrentScene()->GetCamera();
            Vector3 camPos = cam->gameObject->GetComponent<Transform>()->GetPosition();
            Vector3 attPos = p_attachedGameObject->GetComponent<Transform>()->GetPosition();
            float distanceToCamera = Vector3::Distance(camPos, attPos);

            t->SetScale(distanceToCamera * c_sizeBoost);
        }
    }
}


