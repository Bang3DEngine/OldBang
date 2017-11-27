#include "Bang/UICanvas.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
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
    if (canvas) { canvas->_ClearFocus(); }
}

void SetFocus(UIFocusable *focusable)
{
    UICanvas *canvas = UICanvas::GetActive();
    if (canvas) { canvas->_SetFocus(focusable); }
}

void UICanvas::_ClearFocus()
{
    _SetFocus(nullptr);
}

void UICanvas::_SetFocus(UIFocusable *focusable)
{
    if (_GetCurrentFocus() != focusable)
    {
        if (_GetCurrentFocus())
        {
            _GetCurrentFocus()->ClearFocus();
            _GetCurrentFocus()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        p_currentFocus = focusable;
        if (_GetCurrentFocus())
        {
            _GetCurrentFocus()->SetFocus();
            _GetCurrentFocus()->EventEmitter<IDestroyListener>::RegisterListener(this);
        }
    }
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
    m_uiLayoutManager->RebuildLayout( GetGameObject()->GetScene() );

    // Focus
    p_currentFocusMouseOver = nullptr;
    List<UIFocusable*> focusables =
            GetGameObject()->GetComponentsInChildrenOnly<UIFocusable>(true);
    for (UIFocusable *focusable : focusables)
    {
        if (focusable->IsEnabled() && focusable->IsFocusEnabled())
        {
            RectTransform *rt = focusable->GetGameObject()->GetRectTransform();
            if (rt && rt->IsMouseOver())
            {
                // MouseOver update
                if (_GetCurrentFocusMouseOver() != p_currentFocusMouseOver)
                {
                    if (_GetCurrentFocusMouseOver())
                    {
                        _GetCurrentFocusMouseOver()->m_hasMouseOver = false;
                        _GetCurrentFocusMouseOver()->
                            EventEmitter<IDestroyListener>::UnRegisterListener(this);
                    }

                    p_currentFocusMouseOver = focusable;
                    if (_GetCurrentFocusMouseOver())
                    {
                        _GetCurrentFocusMouseOver()->m_hasMouseOver = true;
                        _GetCurrentFocusMouseOver()->
                            EventEmitter<IDestroyListener>::RegisterListener(this);
                    }
                }
                else { p_currentFocusMouseOver = focusable; }

                // Mouse Focus on click
                if (Input::GetMouseButtonDown(MouseButton::Left))
                {
                    _SetFocus(focusable);
                }

                break; // Finished searching!
            }
        }
    }

    if (!p_currentFocusMouseOver && Input::GetMouseButtonDown(MouseButton::Left))
    {
        _ClearFocus(); // Clicked onto nothing, clear focus
    }
}

void UICanvas::OnPostUpdate()
{
    Component::OnPostUpdate();

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
    UIFocusable *destroyedFocusable = SCAST<UIFocusable*>(object);
    if (destroyedFocusable == _GetCurrentFocus())
    {
        p_currentFocus = nullptr;
    }

    if (destroyedFocusable == _GetCurrentFocusMouseOver())
    {
        p_currentFocusMouseOver = nullptr;
    }
}

bool UICanvas::HasFocus(const Component *comp, bool recursive)
{
    return comp ? UICanvas::HasFocus(comp->GetGameObject(), recursive) : false;
}

bool UICanvas::HasFocus(const GameObject *go, bool recursive)
{
    if (!go || !UICanvas::GetCurrentFocusGameObject()) { return false; }

    if (!recursive) { return (UICanvas::GetCurrentFocusGameObject() == go); }
    else { return HasFocus(go, false) ||
                  go->IsChildOf(UICanvas::GetCurrentFocusGameObject()); }
}

bool UICanvas::IsMouseOver(const Component *comp, bool recursive)
{
    return comp ? UICanvas::IsMouseOver(comp->GetGameObject(), recursive) : false;
}

bool UICanvas::IsMouseOver(const GameObject *go, bool recursive)
{
    if (!go || !UICanvas::GetCurrentFocusGameObject()) { return false; }

    if (!recursive) { return (UICanvas::GetCurrentFocusMouseOverGameObject() == go); }
    else { return HasFocus(go, false) ||
                  go->IsChildOf(UICanvas::GetCurrentFocusMouseOverGameObject()); }
}

UIFocusable *UICanvas::GetCurrentFocus()
{
    UICanvas *canvas = UICanvas::GetActive();
    return canvas ? canvas->_GetCurrentFocus() : nullptr;
}

GameObject *UICanvas::GetCurrentFocusGameObject()
{
    UIFocusable *focus = UICanvas::GetCurrentFocus();
    return focus ? focus->GetGameObject() : nullptr;
}

UIFocusable *UICanvas::GetCurrentFocusMouseOver()
{
    UICanvas *canvas = UICanvas::GetActive();
    return canvas ? canvas->_GetCurrentFocusMouseOver() : nullptr;
}

GameObject *UICanvas::GetCurrentFocusMouseOverGameObject()
{
    UIFocusable *focus = UICanvas::GetCurrentFocusMouseOver();
    return focus ? focus->GetGameObject() : nullptr;
}

UIFocusable *UICanvas::_GetCurrentFocus() const
{
    return p_currentFocus;
}

UIFocusable *UICanvas::_GetCurrentFocusMouseOver() const
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
