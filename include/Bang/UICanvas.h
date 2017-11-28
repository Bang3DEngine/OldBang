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
    static bool IsMouseOver(const Component *comp);
    static bool IsMouseOver(const GameObject *go);

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

    Set<IFocusable*> p_currentFocus;
    Set<IFocusable*> p_currentFocusMouseOver;

    void _SetFocus(IFocusable *focusable);
    void _SetFocusMouseOver(IFocusable *focusable);

    Set<IFocusable*>& GetCurrentFocus();
    Set<IFocusable*>& GetCurrentFocusMouseOver();

    void GetSortedFocusCandidates(List<IFocusable*> *sortedCandidates,
                                  const GameObject *go) const;
};

NAMESPACE_BANG_END

#endif // CANVAS_H
