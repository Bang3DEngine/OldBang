#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class UIFocusable;
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
    virtual void OnPostUpdate() override;

    void Invalidate();

    static void ClearFocus();
    static void SetFocus(UIFocusable *focusable);

    void _ClearFocus();
    void _SetFocus(UIFocusable *focusable);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // ISerializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    // Static functions
    static bool HasFocus(const Component *comp, bool recursive = false);
    static bool HasFocus(const GameObject *go, bool recursive = false);
    static bool IsMouseOver(const Component *comp, bool recursive = false);
    static bool IsMouseOver(const GameObject *go, bool recursive = false);
    static UIFocusable *GetCurrentFocus();
    static GameObject *GetCurrentFocusGameObject();
    static UIFocusable *GetCurrentFocusMouseOver();
    static GameObject *GetCurrentFocusMouseOverGameObject();

    UIFocusable *_GetCurrentFocus() const;
    UIFocusable *_GetCurrentFocusMouseOver() const;
    UILayoutManager* GetLayoutManager() const;

    static UICanvas *GetActive();

private:
    static UICanvas *p_activeCanvas;

    UILayoutManager *m_uiLayoutManager = nullptr;
    UIFocusable *p_currentFocus = nullptr;
    UIFocusable *p_currentFocusMouseOver = nullptr;
};

NAMESPACE_BANG_END

#endif // CANVAS_H
