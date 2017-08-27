#ifndef GUITEXTCURSOR_H
#define GUITEXTCURSOR_H

#include "Bang/UIGameObject.h"

FORWARD class LineRenderer;

class GUITextCursor : public UIGameObject
{
    UIGAMEOBJECT(GUITextCursor)

public:
    GUITextCursor();
    virtual ~GUITextCursor();

    virtual void OnUpdate() override;

    void SetStroke(float cursorWidth);
    void SetTickTime(float cursorTickTime);

    float GetStroke() const;
    float GetTickTime() const;

private:
    float m_cursorTime = 0.0f;
    float m_cursorTickTime = 0.25f;
    LineRenderer *m_lineRenderer = nullptr;
};

#endif // GUITEXTCURSOR_H
