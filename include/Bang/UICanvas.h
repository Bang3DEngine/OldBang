#ifndef CANVAS_H
#define CANVAS_H

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

    void Invalidate();

    // Static functions
    static void ClearFocus();
    static void SetFocus(IFocusable *focusable);

    static bool HasFocus(const Component *comp);
    static bool HasFocus(const GameObject *go);
    static bool IsMouseOver(const Component *comp, bool recursive = false);
    static bool IsMouseOver(const GameObject *go, bool recursive = false);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // ISerializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    UILayoutManager* GetLayoutManager() const;

    static UICanvas *GetActive();

private:
    static UICanvas *p_activeCanvas;

    UILayoutManager *m_uiLayoutManager = nullptr;

    IFocusable* p_currentFocus = nullptr;
    IFocusable* p_currentFocusMouseOver = nullptr;

    void ApplyFocusChange();
    void _SetFocus(IFocusable *focusable);
    void _SetFocusMouseOver(IFocusable *focusable);

    IFocusable* GetCurrentFocus();
    IFocusable* GetCurrentFocusMouseOver();

    void GetSortedFocusCandidates(const GameObject *go,
                                  Array<IFocusable *> *sortedCandidates) const;
};

NAMESPACE_BANG_END

#endif // CANVAS_H
