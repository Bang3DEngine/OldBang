#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

FORWARD class UIGameObject;
FORWARD class UITextCursor;
FORWARD class UIScrollArea;
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

    UIGameObject *p_label = nullptr;
    UITextCursor *p_cursor = nullptr;
    UIScrollArea *p_boxScrollArea = nullptr;

    UIGameObject *p_selectionQuad = nullptr;

    int m_cursorIndex = 0;
    bool m_selectingWithMouse = false;
    int m_latestCursorX = 1;
    int m_selectionIndex = 0;

    bool m_forceUpdateRenderers = false;

    void HandleTyping();
    void HandleMouseSelection();
    void HandleKeySelection(bool wasSelecting);
    void HandleCursorIndices(bool wasSelecting);

    float GetCursorXGlobalNDC(int cursorIndex) const;
    // Returns the X in global NDC, for a given cursor index

    float GetCursorXLocalNDC(int cursorIndex) const;
    // Returns the X in local NDC, for a given cursor index

    bool IsDelimiter(char initialChar, char currentChar) const;
    int GetWordSplitIndex(int startingIndex, bool forward) const;

    void UpdateCursorRenderersAndScrolling();
    bool IsShiftPressed() const;

    static UIGameObject *CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

#endif // UIINPUTTEXT_H