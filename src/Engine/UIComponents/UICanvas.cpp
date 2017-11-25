#include "Bang/UICanvas.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UICanvas::UICanvas()
{
    m_uiLayoutManager = new UILayoutManager();
}

UICanvas::~UICanvas()
{
}

void UICanvas::Invalidate()
{
    List<RectTransform*> rts = GetGameObject()->
                               GetComponentsInChildren<RectTransform>(true);
    for (RectTransform *rt : rts)
    {
        rt->Invalidate();
    }

}

void UICanvas::OnStart()
{
    Component::OnStart();
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();
    m_uiLayoutManager->RebuildLayout( GetGameObject()->GetScene() );
}

void UICanvas::OnPostUpdate()
{
    Component::OnPostUpdate();
}

void UICanvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
}

void UICanvas::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);
}

void UICanvas::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
}

UILayoutManager *UICanvas::GetLayoutManager() const
{
    return m_uiLayoutManager;
}
