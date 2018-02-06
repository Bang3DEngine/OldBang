#include "Bang/UICanvas.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Cursor.h"
#include "Bang/XMLNode.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusable.h"
#include "Bang/UIRectMask.h"
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
    delete m_uiLayoutManager;
}

void UICanvas::OnStart()
{
    Component::OnStart();
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();

    // Focus
    IFocusable *focusMouseOver = nullptr;

    Array< std::pair<IFocusable*, Rect> > focusablesAndRectsNDC;
    GetSortedFocusCandidatesByOcclusionOrder(GetGameObject(),
                                             &focusablesAndRectsNDC);
    for (const auto& focusableAndRectNDC : focusablesAndRectsNDC)
    {
        IFocusable *focusable = focusableAndRectNDC.first;
        const Rect& maskedRectNDC = focusableAndRectNDC.second;

        Component *focusableComp = Cast<Component*>(focusable);
        if (focusableComp->IsActive() && focusable->IsFocusEnabled())
        {
            RectTransform *rt = focusableComp->GetGameObject()->GetRectTransform();
            if (rt && rt->IsMouseOver() &&
                maskedRectNDC.Contains( Input::GetMousePositionNDC() ))
            {
                focusMouseOver = focusable;
                if (Input::GetMouseButtonDown(MouseButton::Left))
                {
                    SetFocus(focusable);
                }
                break; // Finished searching!
            }
        }
    }

    // Create list of focusables, and track those destroyed. For this, we create
    // a helper class which implements IDestroyListener
    struct DestroyFocusablesHandler : public IDestroyListener
    {
        Set<IFocusable*> set;
        void OnDestroyed(EventEmitter<IDestroyListener> *object) override
        { set.Add( DCAST<IFocusable*>(object) ); }
    };

    Array<IFocusable*> focusables;
    DestroyFocusablesHandler destroyedFocusables;
    for (const auto& focusableAndRectNDC : focusablesAndRectsNDC)
    {
        IFocusable *focusable = focusableAndRectNDC.first;
        Object *objFocusable = DCAST<Object*>(focusable);
        if (objFocusable)
        {
            objFocusable->EventEmitter<IDestroyListener>::
                         RegisterListener(&destroyedFocusables);
        }
        focusables.PushBack(focusable);
    }

    // Reset focus when clicking out of everything
    if (Input::GetMouseButtonDown(MouseButton::Left) && !focusMouseOver)
    {
        SetFocus(nullptr);
    }
    SetFocusMouseOver(focusMouseOver);

    // Reset cursor type on mouse up
    if (Input::GetMouseButtonUp(MouseButton::Left))
    {
        Cursor::Set(Cursor::Type::Arrow);
    }

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
                newFocusIndex = (newFocusIndex + (shift ? 1 : -1) + n) % n;
                if (newFocusIndex == indexOfFocus) { break; } // Complete loop

                IFocusable *newFocus = focusables.At(newFocusIndex);
                Component *newFocusComp = Cast<Component*>(newFocus);
                const bool isValid = newFocus->IsFocusEnabled() &&
                                     (!newFocusComp ||
                                       newFocusComp->IsEnabled(true));
                if (isValid) { break; }
            }
            SetFocus( focusables.At(newFocusIndex) );
        }
    }

    // Update focusables
    for (IFocusable *focusable : focusables)
    {
        if (!destroyedFocusables.set.Contains(focusable))
        {
            focusable->UpdateFromCanvas();
        }
    }
}

void UICanvas::InvalidateCanvas()
{
    List<RectTransform*> rts = GetGameObject()->
                               GetComponentsInChildren<RectTransform>(true);
    for (RectTransform *rt : rts) { rt->Invalidate(); }
}

void UICanvas::SetFocus(IFocusable *_newFocusable)
{
    IFocusable *newFocusable = _newFocusable;
    Object *obj = newFocusable ? DCAST<Object*>(newFocusable) : nullptr;
    if (obj)
    {
        if (obj->IsWaitingToBeDestroyed()) { newFocusable = nullptr; }
    }

    if (newFocusable != GetCurrentFocus())
    {
        if (GetCurrentFocus())
        {
            Object *focusableObj = Cast<Object*>( GetCurrentFocus() );
            if (GetCurrentFocus() != GetCurrentFocusMouseOver())
            { focusableObj->EventEmitter<IDestroyListener>::UnRegisterListener(this); }

            GetCurrentFocus()->ClearFocus();
        }

        p_currentFocus = newFocusable;
        if (GetCurrentFocus())
        {
            Object *focusableObj = DCAST<Object*>( GetCurrentFocus() );
            focusableObj->EventEmitter<IDestroyListener>::RegisterListener(this);

            GetCurrentFocus()->SetFocus();
        }
    }
}

void UICanvas::SetFocusMouseOver(IFocusable *_newFocusableMO)
{
    IFocusable *newFocusableMO = _newFocusableMO;
    Object *obj = newFocusableMO ? DCAST<Object*>(newFocusableMO) : nullptr;
    if (obj)
    {
        if (obj->IsWaitingToBeDestroyed()) { newFocusableMO = nullptr; }
    }

    if (newFocusableMO != GetCurrentFocusMouseOver())
    {
        Cursor::Set(Cursor::Type::Arrow); // Reset cursor type

        if (GetCurrentFocusMouseOver())
        {
            Object *focusableMOObj = Cast<Object*>( GetCurrentFocusMouseOver() );
            if (GetCurrentFocus() != GetCurrentFocusMouseOver())
            { focusableMOObj->EventEmitter<IDestroyListener>::UnRegisterListener(this); }

            GetCurrentFocusMouseOver()->PropagateMouseOverToListeners(false);
        }

        p_currentFocusMouseOver = newFocusableMO;
        if (GetCurrentFocusMouseOver())
        {
            Object *focusableMOObj = Cast<Object*>( GetCurrentFocusMouseOver() );
            focusableMOObj->EventEmitter<IDestroyListener>::RegisterListener(this);
            GetCurrentFocusMouseOver()->PropagateMouseOverToListeners(true);
        }
    }
}

void UICanvas::OnAfterChildrenUpdate()
{
    Component::OnAfterChildrenUpdate();

    if (GetCurrentFocus()) { GetCurrentFocus()->m_hasJustFocusChanged = false; }
}

void UICanvas::OnBeforeRender()
{
    Component::OnBeforeRender();
    GetLayoutManager()->RebuildLayout( GetGameObject() );
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

void UICanvas::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    IFocusable *destroyedFocusable = DCAST<IFocusable*>(object);

    if (destroyedFocusable == GetCurrentFocus())
    { SetFocus(nullptr); }

    if (destroyedFocusable == GetCurrentFocusMouseOver())
    { SetFocusMouseOver(nullptr); }
}

bool UICanvas::HasFocusFocusable(const IFocusable *focusable)
{
    return GetCurrentFocus() == focusable;
}
bool UICanvas::HasFocus(const Component *comp, bool recursive)
{
    return comp ? UICanvas::HasFocus(comp->GetGameObject(), recursive) : false;
}
bool UICanvas::HasFocus(const GameObject *go, bool recursive)
{
    if (!go) { return false; }

    if (!recursive)
    {
        List<IFocusable*> focusables = go->GetComponents<IFocusable>();
        for (IFocusable *focusable : focusables)
        {
            if (HasFocusFocusable(focusable)) { return true; }
        }
    }
    else
    {
        if (HasFocus(go, false)) { return true; }
        else
        {
            const List<GameObject*>& children = go->GetChildren();
            for (GameObject *child : children)
            {
                if (HasFocus(child, true)) { return true; }
            }
        }
    }
    return false;
}

bool UICanvas::IsMouseOverFocusable(const IFocusable *focusable)
{
    return GetCurrentFocusMouseOver() == focusable;
}
bool UICanvas::IsMouseOver(const Component *comp, bool recursive)
{
    return comp ? UICanvas::IsMouseOver(comp->GetGameObject(), recursive) : false;
}
bool UICanvas::IsMouseOver(const GameObject *go, bool recursive)
{
    if (!go) { return false; }
    if (!recursive)
    {
        List<IFocusable*> focusables = go->GetComponents<IFocusable>();
        for (IFocusable *focusable : focusables)
        {
            if (IsMouseOverFocusable(focusable)) { return true; }
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

UICanvas *UICanvas::GetActive(const GameObject *go)
{
    UICanvas *canvasInThis = go->GetComponent<UICanvas>();
    return canvasInThis ? canvasInThis : go->GetComponentInParent<UICanvas>();
}

UICanvas *UICanvas::GetActive(const Component *comp)
{
    return UICanvas::GetActive(comp->GetGameObject());
}

struct GameObjectZComparer
{
    inline bool operator() (const std::pair<IFocusable*, Rect>& lhs,
                            const std::pair<IFocusable*, Rect>& rhs)
    {
        const GameObject *glhs = Cast<const GameObject*>(lhs.first);
        const GameObject *grhs = Cast<const GameObject*>(rhs.first);
        if (!glhs) { glhs = Cast<const Component*>(lhs.first)->GetGameObject(); }
        if (!grhs) { grhs = Cast<const Component*>(rhs.first)->GetGameObject(); }
        Transform *lt = glhs->GetTransform();
        Transform *rt = grhs->GetTransform();
        if (!lt) { return false; }
        if (!rt) { return true; }
        return lt->GetPosition().z < rt->GetPosition().z;
    }
};


void UICanvas::GetSortedFocusCandidatesByOcclusionOrder(
        const GameObject *go,
        Array< std::pair<IFocusable*, Rect> > *sortedCandidates) const
{
    std::stack<Rect> auxMaskRectStack;
    auxMaskRectStack.push(Rect::NDCRect);

    GetSortedFocusCandidatesByPaintOrder(go, sortedCandidates, &auxMaskRectStack);

    Containers::StableSort(sortedCandidates->Begin(), sortedCandidates->End(),
                           GameObjectZComparer());
}

void UICanvas::GetSortedFocusCandidatesByPaintOrder(
        const GameObject *go,
        Array< std::pair<IFocusable*, Rect> > *sortedCandidates,
        std::stack<Rect> *maskRectStack) const
{
    List<GameObject*> children = go->GetChildren();
    for (auto it = children.RBegin(); it != children.REnd(); ++it)
    {
        GameObject *child = *it;
        if (child->IsActive())
        {
            UIRectMask *rectMask = child->GetComponent<UIRectMask>();
            Rect maskedRectNDC = maskRectStack->top();
            if (rectMask && rectMask->IsActive() && rectMask->IsMasking())
            {
                Rect childRect = child->GetRectTransform()->GetViewportRectNDC();
                maskedRectNDC = Rect::Intersection(maskedRectNDC, childRect);
            }
            maskRectStack->push(maskedRectNDC);

            GetSortedFocusCandidatesByPaintOrder(child, sortedCandidates,
                                                 maskRectStack);

            IFocusable *focusable = child->GetComponent<IFocusable>();
            if (focusable)
            {
                Object *focusableObj = Cast<Object*>(focusable);
                if (!focusableObj || focusableObj->IsActive())
                {
                    sortedCandidates->PushBack(
                            std::make_pair(focusable, maskedRectNDC));
                }
            }
            maskRectStack->pop();
        }
    }
}
