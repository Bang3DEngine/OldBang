#ifndef UIHORIZONTALLAYOUT_H
#define UIHORIZONTALLAYOUT_H

#include "Bang/UIGameObject.h"

class UIHorizontalLayout : public UIGameObject
{
public:
    UIHorizontalLayout();
    virtual ~UIHorizontalLayout();

    void Add(UIGameObject *gameObject, int index = -1);
    void Move(int indexFrom, int indexTo);
    void Remove(int index);
    void Remove(UIGameObject *gameObject);

    void SetStretch(int index, float stretch);
    void SetStretch(UIGameObject *gameObject, float stretch);

    UIGameObject* GetGameObjectAt(int index);

private:
    class UIContainer : public UIGameObject
    {
    public:
        UIContainer(const String& name);
        float stretch = 1.0f;

        UIGameObject *GetGameObject() noexcept;
    };

    List<UIContainer*> m_containers;

    UIContainer* GetContainerAt(int index) noexcept;
    int GetIndexFromUIGameObject(UIGameObject *gameObject) const noexcept;
    void UpdateContainerRectTransforms();
};

#endif // UIHORIZONTALLAYOUT_H
