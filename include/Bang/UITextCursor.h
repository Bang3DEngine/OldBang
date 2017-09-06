#ifndef UITEXTCURSOR_H
#define UITEXTCURSOR_H

#include "Bang/LineRenderer.h"

NAMESPACE_BANG_BEGIN

class UITextCursor : public LineRenderer
{
    COMPONENT(UITextCursor)

public:
    UITextCursor();
    virtual ~UITextCursor();

    virtual void OnUpdate() override;

    void SetStroke(float cursorWidth);
    void SetTickTime(float cursorTickTime);

    float GetStroke() const;
    float GetTickTime() const;

private:
    float m_cursorTime = 0.0f;
    float m_cursorTickTime = 0.25f;
};

NAMESPACE_BANG_END

#endif // UITEXTCURSOR_H
