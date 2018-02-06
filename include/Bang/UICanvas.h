#ifndef CANVAS_H
#define CANVAS_H

#include <stack>

#include "Bang/Set.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class IFocusable;
FORWARD class UILayoutManager;

class UICanvas : public Component,
                 public IDestroyListener
{
    COMPONENT(UICanvas)

public:
    UICanvas();
    virtual ~UICanvas();

    // Component
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnAfterChildrenUpdate() override;
    virtual void OnBeforeRender() override;

    void InvalidateCanvas();

    void ClearFocus();
    void SetFocus(IFocusable *focusable);

    IFocusable* GetCurrentFocus();
    IFocusable* GetCurrentFocusMouseOver();
    bool HasFocusFocusable(const IFocusable *focusable);
    bool HasFocus(const Component *comp, bool recursive = false);
    bool HasFocus(const GameObject *go, bool recursive = false);
    bool IsMouseOverFocusable(const IFocusable *focusable);
    bool IsMouseOver(const Component *comp, bool recursive = false);
    bool IsMouseOver(const GameObject *go, bool recursive = false);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // ISerializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IDestroyListener
    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    UILayoutManager* GetLayoutManager() const;

    static UICanvas *GetActive(const GameObject *go);
    static UICanvas *GetActive(const Component *comp);

private:
    UILayoutManager *m_uiLayoutManager = nullptr;

    IFocusable* p_currentFocus = nullptr;
    IFocusable* p_currentFocusMouseOver = nullptr;

    void ApplyFocusChange();
    void SetFocusMouseOver(IFocusable *focusable);

    void GetSortedFocusCandidatesByOcclusionOrder(
            const GameObject *go,
            Array< std::pair<IFocusable*, AARect> > *sortedCandidates) const;

    void GetSortedFocusCandidatesByPaintOrder(
            const GameObject *go,
            Array< std::pair<IFocusable*, AARect> > *sortedCandidates,
            std::stack<AARect> *maskRectStack) const;
};

NAMESPACE_BANG_END

#endif // CANVAS_H
