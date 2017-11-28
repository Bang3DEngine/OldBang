#include "Bang/UICanvas.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusable.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UICanvas *UICanvas::p_activeCanvas = nullptr;

UICanvas::UICanvas()
{
    m_uiLayoutManager = new UILayoutManager();
}

UICanvas::~UICanvas()
{
}

void UICanvas::OnStart()
{
    Component::OnStart();
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();
    UICanvas::p_activeCanvas = this;

    // Layout rebuilding
    m_uiLayoutManager->RebuildLayout( GetGameObject() );

    // Focus
    p_currentFocusMouseOver.Clear();

    List<IFocusable*> focusables;
    GetSortedFocusCandidates(&focusables, GetGameObject());
    for (IFocusable *focusable : focusables)
    {
        Component *focusableComp = Cast<Component*>(focusable);
        if (focusableComp->IsEnabled() && focusable->IsFocusEnabled())
        {
            RectTransform *rt = focusableComp->GetGameObject()->GetRectTransform();
            if (rt && rt->IsMouseOver())
            {
                _SetFocusMouseOver(focusable);

                if (Input::GetMouseButtonDown(MouseButton::Left))
                {
                    _SetFocus(focusable);
                }

                break; // Finished searching!
            }
        }
    }

    if (GetCurrentFocusMouseOver().IsEmpty() &&
        Input::GetMouseButtonDown(MouseButton::Left))
    {
        _SetFocus(nullptr); // Clicked onto nothing, clear focus
    }

    if (Input::GetKeyDown(Key::X))
    {
        Debug_Log(GetCurrentFocus());
    }
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

void UICanvas::ClearFocus()
{
    UICanvas *canvas = UICanvas::GetActive();
    if (canvas) { canvas->_SetFocus(nullptr); }
}

void UICanvas::SetFocus(IFocusable *focusable)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (canvas) { canvas->_SetFocus(focusable); }
}

void UICanvas::_SetFocus(IFocusable *newFocusable)
{
    Debug_Log("_SetFocus " << newFocusable);
    if (!GetCurrentFocus().Contains(newFocusable))
    {
        for (IFocusable *focusable : GetCurrentFocus())
        {
            Component *focusableComp = Cast<Component*>(newFocusable);
            focusableComp->EventEmitter<IDestroyListener>::UnRegisterListener(this);
            focusable->ClearFocus();
        }
        p_currentFocus.Clear();

        if (newFocusable)
        {
            Component *newFocusableComp = Cast<Component*>(newFocusable);
            List<IFocusable*> newFocus = newFocusableComp->GetGameObject()->
                                  GetComponentsInParent<IFocusable>(true);
            newFocus.PushFront(newFocusable);
            Debug_Log(newFocus);
            p_currentFocus.Add(newFocus.Begin(), newFocus.End());
            for (IFocusable *focusable : GetCurrentFocus())
            {
                Component *focusableComp = Cast<Component*>(focusable);
                focusable->SetFocus();
                focusableComp->EventEmitter<IDestroyListener>::RegisterListener(this);
            }
        }
    }
    Debug_Log("_SetFocus end: " << GetCurrentFocus());
}

void UICanvas::_SetFocusMouseOver(IFocusable *newFocusableMO)
{
    if (!GetCurrentFocusMouseOver().Contains(newFocusableMO))
    {
        for (IFocusable *focusable : GetCurrentFocusMouseOver())
        {
            Component *focusableComp = Cast<Component*>(focusable);
            focusableComp->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }
        p_currentFocusMouseOver.Clear();

        if (newFocusableMO)
        {
            Component *newFocusablMOComp = Cast<Component*>(newFocusableMO);
            List<IFocusable*> newFocusMO = newFocusablMOComp->GetGameObject()->
                                  GetComponentsInParent<IFocusable>(true);
            newFocusMO.PushFront(newFocusableMO);

            p_currentFocusMouseOver.Add(newFocusMO.Begin(), newFocusMO.End());
            for (IFocusable *focusable : GetCurrentFocusMouseOver())
            {
                Component *focusableComp = Cast<Component*>(focusable);
                focusableComp->EventEmitter<IDestroyListener>::RegisterListener(this);
            }
        }
    }
}
void UICanvas::OnAfterChildrenUpdate()
{
    Component::OnAfterChildrenUpdate();
    UICanvas::p_activeCanvas = nullptr;
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

void UICanvas::OnDestroyed(Object *object)
{
    IFocusable *destroyedFocusable = Cast<IFocusable*>(object);
    GetCurrentFocusMouseOver().Remove(destroyedFocusable);
    GetCurrentFocus().Remove(destroyedFocusable);
}

bool UICanvas::HasFocus(const Component *comp)
{
    return comp ? UICanvas::HasFocus(comp->GetGameObject()) : false;
}

bool UICanvas::HasFocus(const GameObject *go)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (!go || !canvas) { return false; }
    IFocusable *focusable = go->GetComponent<IFocusable>();
    return canvas->GetCurrentFocus().Contains(focusable);
}

bool UICanvas::IsMouseOver(const Component *comp)
{
    return comp ? UICanvas::IsMouseOver(comp->GetGameObject()) : false;
}

bool UICanvas::IsMouseOver(const GameObject *go)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (!go || !canvas) { return false; }
    IFocusable *focusable = go->GetComponent<IFocusable>();
    return canvas->GetCurrentFocusMouseOver().Contains(focusable);
}

Set<IFocusable*>& UICanvas::GetCurrentFocus()
{
    return p_currentFocus;
}

Set<IFocusable*>& UICanvas::GetCurrentFocusMouseOver()
{
    return p_currentFocusMouseOver;
}

UILayoutManager *UICanvas::GetLayoutManager() const
{
    return m_uiLayoutManager;
}

UICanvas *UICanvas::GetActive()
{
    return UICanvas::p_activeCanvas;
}

void UICanvas::GetSortedFocusCandidates(List<IFocusable *> *sortedCandidates,
                                        const GameObject *go) const
{
    for (GameObject *child : go->GetChildren())
    {
        if (child->IsEnabled())
        {
            GetSortedFocusCandidates(sortedCandidates, child);

            IFocusable *focusable = child->GetComponent<IFocusable>();
            if (focusable) { sortedCandidates->PushBack(focusable); }
        }
    }
}
