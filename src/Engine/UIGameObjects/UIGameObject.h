#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "Bang/GameObject.h"

FORWARD class RectTransform;

class UIGameObject : public GameObject
{
public:
    UIGameObject(const String& name = "");

    RectTransform *const& rectTransform = m_rectTransform;

    bool IsMouseOver() const;

private:
    RectTransform *m_rectTransform = nullptr;
};

#endif // UIGAMEOBJECT_H
