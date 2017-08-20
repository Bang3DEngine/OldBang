#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/UIGameObject.h"

FORWARD class GUILabel;
FORWARD class GUIScrollArea;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class SingleLineRenderer;

class GUIInputText : public UIGameObject
{
public:
    GUIInputText();
    virtual ~GUIInputText();

    void OnUpdate() override;

    void SetCursorPosition(int index);
    void SetSelection(int selectionBeginIndex,
                      int selectionEndIndex);
    String GetSelectedText() const;

    void SetCursorWidth(float cursorWidth);
    void SetCursorTickTime(float cursorTickTime);

    bool GetMasking() const;
    float GetCursorWidth() const;
    float GetCursorTickTime() const;

    void ResetSelection();
    void SelectAll();

    UITextRenderer *GetText() const;
    UIImageRenderer *GetBackground() const;

private:
    static const Vector2i LookAheadOffsetPx;

    UIImageRenderer *p_background = nullptr;

    GUILabel *m_label = nullptr;
    GUIScrollArea *m_boxScrollArea = nullptr;
    SingleLineRenderer *m_cursorRenderer = nullptr;

    UIGameObject *p_selectionQuad = nullptr;

    int m_cursorIndex = 0;
    bool m_selectingWithMouse = false;
    int m_latestCursorX = 1;
    int m_selectionIndex = 0;

    float m_cursorTickTime = 0.25f;
    float m_cursorTime = 0.0f;

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
