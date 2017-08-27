#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Map.h"
#include "Bang/UIGameObject.h"

class GUIDirLayout : public UIGameObject
{
    UIGAMEOBJECT(GUIDirLayout)

public:
    void Add(UIGameObject *gameObject, int index = -1);
    void Move(int indexFrom, int indexTo);
    UIGameObject* Take(int index);
    UIGameObject* Take(UIGameObject *gameObject);

    void SetSpacing(int spacingPx);
    void SetStretch(int index, float stretch);
    void SetStretch(UIGameObject *gameObject, float stretch);

protected:
    GUIDirLayout();
    GUIDirLayout(const String &name, bool vertical);
    virtual ~GUIDirLayout();

private:
    class UIContainer : public UIGameObject
    {
    public:
        UIContainer(UIGameObject *child) : UIGameObject()
        {
            child->SetParent(this);
        }

        float stretch = 1.0f;
    };

    int m_spacingPx = 0;
    bool m_vertical = false;
    Map<UIGameObject*, UIContainer*> m_childrenContainers;

    int GetIndexFromChild(UIGameObject *gameObject) const noexcept;
    void UpdateChildrenRectTransforms();
};

#endif // UIDIRLAYOUT_H
