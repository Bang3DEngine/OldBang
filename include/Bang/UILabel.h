#ifndef UILABEL_H
#define UILABEL_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIMask;
FORWARD class RectTransform;
FORWARD class UITextRenderer;

class UILabel : public IComponentDriver<UILabel>,
                public Component
{
    COMPONENT(UILabel)

public:
	virtual ~UILabel();

    void OnUpdate() override;

    void SetCursorIndex(int index);
    void SetSelectionIndex(int index);
    void SetSelection(int beginIndex, int endIndex);
    String GetSelectedText() const;
    void ResetSelection();

    int GetCursorIndex() const;
    int GetSelectionIndex() const;
    int GetSelectionBeginIndex() const;
    int GetSelectionEndIndex() const;

    float GetCursorXGlobalNDC(int cursorIndex) const;
    float GetCursorXLocalNDC(int cursorIndex) const;

    UIMask *GetMask() const;
    UITextRenderer *GetText() const;

private:
    int m_cursorIndex = 0;
    int m_selectionIndex = 0;

    UIMask *p_mask = nullptr;
    UITextRenderer *p_text = nullptr;
    GameObject *p_selectionQuad = nullptr;

    UILabel();

    RectTransform *GetParentRT() const;

    // IComponentDriver
    static UILabel *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UILabel>;
};

NAMESPACE_BANG_END

#endif // UILABEL_H_H

