#include "Bang/GUITextCursor.h"

#include "Bang/Time.h"
#include "Bang/Material.h"
#include "Bang/LineRenderer.h"
#include "Bang/RectTransform.h"

GUITextCursor::GUITextCursor() : UIGameObject("GUITextCursor")
{
    m_lineRenderer = AddComponent<LineRenderer>();
    m_lineRenderer->UseMaterialCopy();
    m_lineRenderer->SetEnabled(false);
    m_lineRenderer->GetMaterial()->SetDiffuseColor(Color::Black);
    m_lineRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    m_lineRenderer->SetRenderPass(RenderPass::Canvas);
    SetStroke(1.5f);

    m_lineRenderer->SetPoints({
                               Vector3(0.0f, -1.0f, 0.0f),
                               Vector3(0.0f,  1.0f, 0.0f),

                               Vector3(-1.0f, -1.0f, 0.0f),
                               Vector3( 1.0f, -1.0f, 0.0f),

                               Vector3(-1.0f,  1.0f, 0.0f),
                               Vector3( 1.0f,  1.0f, 0.0f),
                              });
}

GUITextCursor::~GUITextCursor()
{

}

void GUITextCursor::OnUpdate()
{
    UIGameObject::OnUpdate();

    m_cursorTime += Time::GetDeltaTime();
    m_lineRenderer->SetEnabled( m_cursorTime <= m_cursorTickTime );
    if (m_cursorTime >= m_cursorTickTime * 2) { m_cursorTime = 0.0f; }
}

void GUITextCursor::SetStroke(float cursorWidth)
{
    m_lineRenderer->SetLineWidth(cursorWidth);
}

void GUITextCursor::SetTickTime(float cursorTickTime)
{
    m_cursorTickTime = cursorTickTime;
}

float GUITextCursor::GetStroke() const
{
    return m_lineRenderer->GetLineWidth();
}

float GUITextCursor::GetTickTime() const
{
    return m_cursorTickTime;
}
