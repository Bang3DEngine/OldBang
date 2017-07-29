#ifndef UIINPUTTEXT_H
#define UIINPUTTEXT_H

#include "Bang/GameObject.h"

FORWARD class UIText;
FORWARD class UIImage;
FORWARD class SingleLineRenderer;

class UIInputText : public GameObject
{
public:
    UIInputText();
    virtual ~UIInputText();

    void OnUpdate() override;

    const String& GetContent() const;

    void HandleKeys();

    void SetMargins(int left, int top, int right, int bot);

    void SetCursorWidth(float cursorWidth);
    float GetCursorWidth() const;

    void SetCursorTickTime(float cursorTickTime);
    float GetCursorTickTime() const;

    UIText *GetUIText() const;
    UIImage *GetBackgroundImage() const;

private:
    UIImage *p_background = nullptr;

    GameObject *m_textContainer = nullptr;
    UIText *p_text = nullptr;
    SingleLineRenderer *m_cursorRenderer = nullptr;

    int m_cursorIndex = 0;
    int m_latestCursorPos = 1;

    float m_cursorTickTime = 0.25f;
    float m_cursorTime = 0.0f;

    float GetCurrentCursorXNDC() const;
    void UpdateCursorRenderer();
};

#endif // UIINPUTTEXT_H
