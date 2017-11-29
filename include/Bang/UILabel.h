#ifndef UILABEL_H
#define UILABEL_H

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIMask;
FORWARD class RectTransform;
FORWARD class UITextRenderer;

class UILabel : public Component,
                public IFocusListener
{
    COMPONENT(UILabel)

public:
	virtual ~UILabel();

    void OnStart() override;
    void OnUpdate() override;

    void SetCursorIndex(int index);
    void SetSelectionIndex(int index);
    void SetSelectable(bool selectable);
    void SetSelection(int cursorIndex, int selectionIndex);
    String GetSelectedText() const;

    void SelectAll();
    void ResetSelection();
    void SetSelectAllOnFocus(bool selectAllOnFocus);

    bool IsSelectable() const;
    int GetCursorIndex() const;
    int GetSelectionIndex() const;
    int GetSelectionBeginIndex() const;
    int GetSelectionEndIndex() const;

    float GetCursorXGlobalNDC(int cursorIndex) const;
    float GetCursorXLocalNDC(int cursorIndex) const;

    bool IsSelectAllOnFocus() const;
    bool IsSelectingWithMouse() const;

    UIMask *GetMask() const;
    UITextRenderer *GetText() const;

    // IFocusListener
    virtual void OnFocusTaken(IFocusable *focusable) override;
    virtual void OnFocusLost(IFocusable *focusable) override;

private:
    int m_cursorIndex = 0;
    int m_selectionIndex = 0;
    bool m_selectable = false;
    bool m_firstSelectAll = true;
    bool m_selectingWithMouse = false;
    bool m_selectAllOnFocusTaken = false;

    UIMask *p_mask = nullptr;
    UITextRenderer *p_text = nullptr;
    GameObject *p_selectionQuad = nullptr;

    UILabel();

    int GetClosestCharIndexTo(const Vector2 &coordsLocalNDC);
    void HandleClipboardCopy();
    void HandleMouseSelection();
    void UpdateSelectionQuadRenderer();

    bool IsShiftPressed() const;
    RectTransform *GetTextParentRT() const;

    static UILabel *CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UILABEL_H_H

