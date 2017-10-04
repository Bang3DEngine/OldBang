#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class UITextCursor;
FORWARD class UIScrollArea;
FORWARD class RectTransform;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;

class UIInputText : public Component,
                    public IFocusListener
{
    COMPONENT(UIInputText)

public:
    UIInputText();
    virtual ~UIInputText();

    void OnUpdate() override;

    void SetCursorPosition(int index);
    void SetSelection(int selectionBeginIndex,
                      int selectionEndIndex);

    int GetSelectionBeginIndex() const;
    int GetSelectionEndIndex() const;
    String GetSelectedText() const;
    void ReplaceSelectedText(const String &replaceStr);

    void ResetSelection();
    void SelectAll();

    // IFocusListener
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

    UITextCursor *GetCursor() const;
    UITextRenderer *GetText() const;
    UIImageRenderer *GetBackground() const;

private:
    static const Vector2i LookAheadOffsetPx;

    UIImageRenderer *p_background = nullptr;

    GameObject *p_label = nullptr;
    UITextCursor *p_cursor = nullptr;
    UIScrollArea *p_boxScrollArea = nullptr;

    GameObject *p_selectionQuad = nullptr;

    int m_cursorIndex = 0;
    bool m_selectingWithMouse = false;
    int m_latestCursorX = 1;
    int m_selectionIndex = 0;

    bool m_forceUpdateRenderers = false;

    void HandleTyping();
    void HandleMouseSelection();
    void HandleKeySelection(bool wasSelecting);
    void HandleCursorIndices(bool wasSelecting);

    RectTransform *GetTextRT() const;
    RectTransform *GetLabelRT() const;

    float GetCursorXGlobalNDC(int cursorIndex) const;
    // Returns the X in global NDC, for a given cursor index

    float GetCursorXLocalNDC(int cursorIndex) const;
    // Returns the X in local NDC, for a given cursor index

    bool IsDelimiter(char initialChar, char currentChar) const;
    int GetWordSplitIndex(int startingIndex, bool forward) const;

    void UpdateCursorRenderersAndScrolling();
    bool IsShiftPressed() const;

    static GameObject *CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIINPUTTEXT_H
