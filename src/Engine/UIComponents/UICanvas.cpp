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

    IFocusable *focusMouseOver = nullptr;

    Array<IFocusable*> focusables;
    GetSortedFocusCandidates(GetGameObject(), &focusables);
    for (IFocusable *focusable : focusables)
    {
        Component *focusableComp = Cast<Component*>(focusable);
        if (focusableComp->IsEnabled() && focusable->IsFocusEnabled())
        {
            RectTransform *rt = focusableComp->GetGameObject()->GetRectTransform();
            if (rt && rt->IsMouseOver())
            {
                focusMouseOver = focusable;

                if (Input::GetMouseButtonDown(MouseButton::Left))
                {
                    _SetFocus(focusable);
                }

                break; // Finished searching!
            }
        }
    }

    if (!GetCurrentFocusMouseOver() &&
        Input::GetMouseButtonDown(MouseButton::Left))
    {
        _SetFocus(nullptr); // Clicked onto nothing, clear focus
    }
    _SetFocusMouseOver(focusMouseOver);


    // Tabbing
    if (Input::GetKeyDownRepeat(Key::Tab))
    {
        IFocusable *currentFocus = GetCurrentFocus();
        if (currentFocus)
        {
            const int n = focusables.Size();
            int indexOfFocus = focusables.IndexOf(currentFocus);
            bool shift = Input::GetKey(Key::LShift) || Input::GetKey(Key::RShift);
            int newFocusIndex = indexOfFocus;

            while (true)
            {
                newFocusIndex = (newFocusIndex + (shift ? -1 : 1) + n) % n;
                if (newFocusIndex == indexOfFocus) { break; } // Complete loop

                IFocusable *newFocus = focusables.At(newFocusIndex);
                Component *newFocusComp = Cast<Component*>(newFocus);
                const bool isValid = newFocus->IsFocusEnabled() &&
                                     (!newFocusComp ||
                                       newFocusComp->IsEnabled(true));
                if (isValid) { break; }
            }
            _SetFocus( focusables.At(newFocusIndex) );
        }
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
    if (newFocusable != GetCurrentFocus())
    {
        if (GetCurrentFocus())
        {
            Component *focusableComp = Cast<Component*>( GetCurrentFocus() );
            focusableComp->EventEmitter<IDestroyListener>::UnRegisterListener(this);

            GetCurrentFocus()->ClearFocus();
        }

        p_currentFocus = newFocusable;
        if (GetCurrentFocus())
        {
            Component *focusableComp = Cast<Component*>( GetCurrentFocus() );
            focusableComp->EventEmitter<IDestroyListener>::RegisterListener(this);

            p_currentFocus->SetFocus();
        }
    }
}

void UICanvas::_SetFocusMouseOver(IFocusable *newFocusableMO)
{
    if (newFocusableMO != GetCurrentFocusMouseOver())
    {
        if (GetCurrentFocusMouseOver())
        {
            Component *focusableMOComp = Cast<Component*>( GetCurrentFocusMouseOver() );
            focusableMOComp->EventEmitter<IDestroyListener>::UnRegisterListener(this);
            GetCurrentFocusMouseOver()->PropagateMouseOverToListeners(false);
        }

        p_currentFocusMouseOver = newFocusableMO;
        if (GetCurrentFocusMouseOver())
        {
            Component *focusableMOComp = Cast<Component*>( GetCurrentFocusMouseOver() );
            focusableMOComp->EventEmitter<IDestroyListener>::RegisterListener(this);
            GetCurrentFocusMouseOver()->PropagateMouseOverToListeners(true);
        }
    }
}

void UICanvas::OnAfterChildrenUpdate()
{
    Component::OnAfterChildrenUpdate();
    UICanvas::p_activeCanvas = nullptr;

    if (GetCurrentFocus()) { GetCurrentFocus()->m_hasJustFocusChanged = false; }
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

    if (destroyedFocusable == GetCurrentFocus())
    { _SetFocus(nullptr); }

    if (destroyedFocusable == GetCurrentFocusMouseOver())
    { _SetFocusMouseOver(nullptr); }
}

bool UICanvas::HasFocus(const Component *comp)
{
    return comp ? UICanvas::HasFocus(comp->GetGameObject()) : false;
}

bool UICanvas::HasFocus(const GameObject *go)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (!go || !canvas) { return false; }
    List<IFocusable*> focusables = go->GetComponents<IFocusable>();
    for (IFocusable *focusable : focusables)
    {
        if (focusable == canvas->GetCurrentFocus()) { return true; }
    }
    return false;
}

bool UICanvas::IsMouseOver(const Component *comp, bool recursive)
{
    return comp ? UICanvas::IsMouseOver(comp->GetGameObject(), recursive) : false;
}

bool UICanvas::IsMouseOver(const GameObject *go, bool recursive)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (!go || !canvas) { return false; }
    if (!recursive)
    {
        List<IFocusable*> focusables = go->GetComponents<IFocusable>();
        for (IFocusable *focusable : focusables)
        {
            if (focusable == canvas->GetCurrentFocusMouseOver()) { return true; }
        }
    }
    else
    {
        if (IsMouseOver(go, false)) { return true; }
        else
        {
            const List<GameObject*>& children = go->GetChildren();
            for (GameObject *child : children)
            {
                if (IsMouseOver(child, true)) { return true; }
            }
        }
    }
    return false;
}

IFocusable *UICanvas::GetCurrentFocus() { return p_currentFocus; }
IFocusable* UICanvas::GetCurrentFocusMouseOver() { return p_currentFocusMouseOver; }

UILayoutManager *UICanvas::GetLayoutManager() const
{
    return m_uiLayoutManager;
}

UICanvas *UICanvas::GetActive()
{
    return UICanvas::p_activeCanvas;
}

void UICanvas::GetSortedFocusCandidates(const GameObject *go,
                                        Array<IFocusable*> *sortedCandidates) const
{
    for (GameObject *child : go->GetChildren())
    {
        if (child->IsEnabled())
        {
            GetSortedFocusCandidates(child, sortedCandidates);

            IFocusable *focusable = child->GetComponent<IFocusable>();
            if (focusable)
            {
                Object *focusableObj = Cast<Object*>(focusable);
                if (!focusableObj ||
                     (focusableObj->IsEnabled() &&
                     !focusableObj->IsWaitingToBeDestroyed())
                   )
                {
                    sortedCandidates->PushBack(focusable);
                }
            }
        }
    }
}
