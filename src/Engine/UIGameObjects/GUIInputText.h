#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/UIGameObject.h"

FORWARD class UIText;
FORWARD class UIImage;
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
    float GetCursorWidth() const;

    void SetCursorTickTime(float cursorTickTime);
    float GetCursorTickTime() const;

    void Update();
    void ResetSelection();

    UIText *GetText() const;
    UIGameObject *GetTextContainer() const;
    UIImage *GetBackground() const;

private:
    UIImage *p_background = nullptr;

    UIGameObject *m_textContainer = nullptr;
    UIText *p_text = nullptr;
    SingleLineRenderer *m_cursorRenderer = nullptr;

    UIGameObject *p_selectionGO = nullptr;
    UIImage *p_selectionQuad = nullptr;

    int m_cursorIndex = 0;
    bool m_selectingWithMouse = false;
    int m_latestCursorIndex = 1;
    int m_selectionCursorIndex = 0;

    float m_cursorTickTime = 0.25f;
    float m_cursorTime = 0.0f;

    bool m_forceUpdateRenderers = false;

    void HandleTyping();
    void HandleTextScrolling();
    void HandleMouseSelection();
    void HandleKeySelection(bool wasSelecting);
    void HandleCursorIndices(bool wasSelecting);

    float GetCursorX_NDC(int cursorIndex) const;

    bool IsDelimiter(char initialChar, char currentChar) const;
    int GetWordSplitIndex(int startingIndex, bool forward) const;

    void UpdateCursorRenderers();
    bool IsShiftPressed() const;
    Vector2 GetSideCursorMarginsNDC() const;
    int GetVisibilityFrontierCharIndex(bool right) const;
};

#endif // UIINPUTTEXT_H
