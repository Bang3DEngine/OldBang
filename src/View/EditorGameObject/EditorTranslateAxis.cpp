#include "EditorTranslateAxis.h"

#include "Toolbar.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir,
                                         GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorTranslateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_line = AddComponent<SingleLineRenderer>();
    m_line->SetDestiny(m_oAxisDirection);
    m_line->SetMaterial(m_material);
    m_line->SetLineWidth(2.0f);
    m_line->SetReceivesLighting(false);

    m_line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );

    m_axisCap = MeshFactory::GetConeEditorGameObject();
    m_axisCap->SetParent(this);
    if (dir == EditorAxis::EditorAxisDirection::X)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(90.0f, 90.0f, 0.0f));
    }
    else if (dir == EditorAxis::EditorAxisDirection::Y)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(0.0f, 0.0f, 0.0f));
    }
    else if (dir == EditorAxis::EditorAxisDirection::Z)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(-90.0f, 0.0f, 0.0f));
    }
    m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
    m_axisCap->transform->SetLocalScale(Vector3(1,2,1) * 0.25f);
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->SetReceivesLighting(true);
}

EditorTranslateAxis::~EditorTranslateAxis()
{

}


void EditorTranslateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->transform;
    GameObject *ago = m_attachedGameObject;
    Vector3 wCamPos = camTransform->GetPosition();

    if (m_grabbed)
    {
        Vector2 sMouseDelta = Input::GetMouseDelta() * Vector2(1.0f, -1.0f);
        if (sMouseDelta.Length() > 0.0f)
        {
            Vector3 wAxisDir, parentAxisDir;
            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                wAxisDir = m_oAxisDirection;
                parentAxisDir = ago->transform->WorldToLocalDirection(m_oAxisDirection);
            }
            else
            {
                wAxisDir = ago->transform->ObjectToWorldDirection(m_oAxisDirection);
                parentAxisDir = ago->transform->ObjectToLocalDirection(m_oAxisDirection);
            }
            wAxisDir.Normalize();
            parentAxisDir.Normalize();

            // Alignment
            Vector3 wAxisCenter = transform->GetPosition();
            Vector2 screenAxisDir = cam->WorldToScreenNDCPoint(wAxisCenter + wAxisDir) -
                                    cam->WorldToScreenNDCPoint(wAxisCenter);
            screenAxisDir.Normalize();
            float alignment = Vector2::Dot(screenAxisDir, sMouseDelta.Normalized());
            //

            Vector3 worldMove = alignment *
                                parentAxisDir *
                                sMouseDelta.Length() *
                                Vector3::Distance(wCamPos, ago->transform->GetPosition()) * 0.002f;
            worldMove *= 1.0f / ago->parent->transform->GetScale();
            ago->transform->TranslateLocal(worldMove);
        }
    }
}

void EditorTranslateAxis::OnDrawGizmosNoDepth()
{
    m_line->Render();
    m_axisCap->GetComponent<Renderer>()->Render();
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return m_line;
}

