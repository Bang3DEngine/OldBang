#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/UIGameObject.h"

FORWARD class GUILabel;
FORWARD class GUITextCursor;
FORWARD class GUIScrollArea;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;

class GUIInputText : public UIGameObject
{
    SERIALIZABLE_OBJECT(GUIInputText)

public:
    GUIInputText();
    virtual ~GUIInputText();

    void OnUpdate() override;

    void SetCursorPosition(int index);
    void SetSelection(int selectionBeginIndex,
                      int selectionEndIndex);

    int GetSelectionBeginIndex() const;
    int GetSelectionEndIndex() const;
    String GetSelectedText() const;

    void ResetSelection();
    void SelectAll();

    GUITextCursor *GetCursor() const;
    UITextRenderer *GetText() const;
    UIImageRenderer *GetBackground() const;

private:
    static const Vector2i LookAheadOffsetPx;

    UIImageRenderer *p_background = nullptr;

    GUILabel *m_label = nullptr;
    GUITextCursor *m_cursor = nullptr;
    GUIScrollArea *m_boxScrollArea = nullptr;

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

    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;
};

#endif // UIINPUTTEXT_H
