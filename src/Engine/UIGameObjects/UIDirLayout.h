#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Map.h"
#include "Bang/UIGameObject.h"

class UIDirLayout : public UIGameObject
{
public:
    void OnUpdate() override;

    void Add(UIGameObject *gameObject, int index = -1);
    void Move(int indexFrom, int indexTo);
    UIGameObject* Take(int index);
    UIGameObject* Take(UIGameObject *gameObject);

    void SetStretch(int index, float stretch);
    void SetStretch(UIGameObject *gameObject, float stretch);

protected:
    UIDirLayout(bool vertical);
    virtual ~UIDirLayout();

private:
    struct ChildInfo
    {
        float stretch = 1.0f;
    };

    bool m_vertical = false;
    Map<UIGameObject*, ChildInfo> m_childrenInfo;

    int GetIndexFromChild(UIGameObject *gameObject) const noexcept;
    ChildInfo* GetChildInfoAt(int index);
    void UpdateChildrenRectTransforms();
};

#endif // UIDIRLAYOUT_H
