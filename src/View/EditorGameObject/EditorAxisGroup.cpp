#include "EditorAxisGroup.h"

EditorAxisGroup::EditorAxisGroup(GameObject *attachedGameObject) : EditorGameObject("EditorAxisGroup")
{
    this->attachedGameObject = attachedGameObject;
    etag = new EditorTranslateAxisGroup(attachedGameObject);
    erag = new EditorRotateAxisGroup(attachedGameObject);
    esag = new EditorScaleAxisGroup(attachedGameObject);

    AddChild(etag);
    AddChild(erag);
    AddChild(esag);

    DisableAllGroups();

    AddComponent<Transform>();
}

void EditorAxisGroup::DisableAllGroups()
{
    etag->SetEnabled(false);
    erag->SetEnabled(false);
    esag->SetEnabled(false);
}

void EditorAxisGroup::OnUpdate()
{
    Toolbar::TransformMode tm = Toolbar::GetInstance()->
                                GetSelectedTransformMode();

    if (tm == Toolbar::TransformMode::Translate)
    {
        DisableAllGroups();
        etag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Rotate)
    {
        DisableAllGroups();
        erag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Scale)
    {
        DisableAllGroups();
        esag->SetEnabled(true);
    }

    //Attached GameObject positioning
    if (attachedGameObject)
    {
        Transform *at = attachedGameObject->GetComponent<Transform>();
        if (at)
        {

            Transform *t = GetComponent<Transform>();

            t->SetPosition(at->GetPosition()); // Pivot
            Box bbox = attachedGameObject->GetBoundingBox();
            t->SetPosition( bbox.GetCenter() ); // Center

            if (Toolbar::GetInstance()->GetGlobalCoordsMode())
            {
                t->SetLocalRotation( Quaternion() );
            }
            else
            {
                t->SetLocalRotation( at->GetRotation() );
            }

            Camera *cam = Canvas::GetCurrentScene()->GetCamera();
            Vector3 camPos = cam->gameObject->GetComponent<Transform>()->GetPosition();
            Vector3 attPos = attachedGameObject->GetComponent<Transform>()->GetPosition();
            float distanceToCamera = Vector3::Distance(camPos, attPos);

            t->SetScale(distanceToCamera * sizeBoost);
        }
    }
}


