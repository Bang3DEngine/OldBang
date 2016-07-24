#include "EditorAxisGroup.h"

EditorAxisGroup::EditorAxisGroup(GameObject *attachedGameObject) : EditorGameObject("EditorAxisGroup")
{
    this->m_attachedGameObject = attachedGameObject;
    m_etag = new EditorTranslateAxisGroup(attachedGameObject);
    m_erag = new EditorRotateAxisGroup(attachedGameObject);
    m_esag = new EditorScaleAxisGroup(attachedGameObject);

    m_etag->SetParent(this);
    m_erag->SetParent(this);
    m_esag->SetParent(this);

    DisableAllGroups();
}

void EditorAxisGroup::DisableAllGroups()
{
    m_etag->SetEnabled(false);
    m_erag->SetEnabled(false);
    m_esag->SetEnabled(false);
}

void EditorAxisGroup::OnUpdate()
{
    Toolbar::TransformMode tm = Toolbar::GetInstance()->
                                GetSelectedTransformMode();

    if (tm == Toolbar::TransformMode::Translate)
    {
        DisableAllGroups();
        m_etag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Rotate)
    {
        DisableAllGroups();
        m_erag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Scale)
    {
        DisableAllGroups();
        m_esag->SetEnabled(true);
    }

    //Attached GameObject positioning
    if (m_attachedGameObject)
    {
        Transform *at = m_attachedGameObject->GetTransform();
        if (at)
        {
            GetTransform()->SetPosition(at->GetPosition()); // Pivot
            Box bbox = m_attachedGameObject->GetBoundingBox();
            GetTransform()->SetPosition( bbox.GetCenter() ); // Center

            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                GetTransform()->SetLocalRotation( Quaternion() );
            }
            else
            {
                GetTransform()->SetLocalRotation( at->GetRotation() );
            }

            Camera *cam = Canvas::GetCurrentScene()->GetCamera();
            Vector3 camPos = cam->gameObject->GetTransform()->GetPosition();
            Vector3 attPos = at->GetPosition();
            float distanceToCamera = Vector3::Distance(camPos, attPos);

            GetTransform()->SetScale(distanceToCamera * c_sizeBoost);
        }
    }
}


