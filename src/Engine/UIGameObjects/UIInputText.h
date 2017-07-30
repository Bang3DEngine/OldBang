#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/UIGameObject.h"

FORWARD class UIText;
FORWARD class UIImage;
FORWARD class SingleLineRenderer;

class UIInputText : public UIGameObject
{
public:
    UIInputText();
    virtual ~UIInputText();

    void OnUpdate() override;

    const String& GetContent() const;

    void HandleTyping();
    void HandleMouseSelection();
    void HandleKeySelection(bool wasSelecting);
    void HandleSelectionIndices(bool wasSelecting);

    void SetMargins(int left, int top, int right, int bot);

    void SetCursorWidth(float cursorWidth);
    float GetCursorWidth() const;

    void SetCursorTickTime(float cursorTickTime);
    float GetCursorTickTime() const;

    void ResetSelection();

    UIText *GetUIText() const;
    UIImage *GetBackgroundImage() const;

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

    float GetCursorX_NDC(int cursorIndex) const;
    void UpdateCursorRenderers();
};

#endif // UIINPUTTEXT_H
