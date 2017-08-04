#include "Bang/UIHorizontalLayout.h"

#include "Bang/Array.h"
#include "Bang/RectTransform.h"

UIHorizontalLayout::UIHorizontalLayout()
{
}

UIHorizontalLayout::~UIHorizontalLayout()
{
}

void UIHorizontalLayout::Add(UIGameObject *gameObject, int _index)
{
    UIContainer *cont = new UIContainer("UIHLayout_Container");
    cont->SetParent(this);

    int index = (_index >= 0) ? _index : m_containers.Size();
    auto it = m_containers.Begin(); std::advance(it, index);
    m_containers.Insert(it, cont);

    gameObject->SetParent(cont);
    UpdateContainerRectTransforms();
}

void UIHorizontalLayout::Move(int indexFrom, int indexTo)
{
    auto it = m_containers.Begin(); std::advance(it, indexFrom);
    Remove(indexFrom);

    UIGameObject *gameObject = (*it)->GetGameObject();
    Add(gameObject, indexTo-1);
}

void UIHorizontalLayout::Remove(int index)
{
    if (index >= 0 && index < m_containers.Size())
    {
        auto it = m_containers.Begin(); std::advance(it, index);
        m_containers.Remove(it);
    }
    else { Debug_Warn("Index " << index << " out of range."); }
    UpdateContainerRectTransforms();
}

void UIHorizontalLayout::Remove(UIGameObject *gameObject)
{
    int index = GetIndexFromUIGameObject(gameObject); ENSURE(index >= 0);
    if (index < m_containers.Size()) { Remove(index); }
}

void UIHorizontalLayout::SetStretch(int index, float stretch)
{
    UIContainer *cont = GetContainerAt(index); ENSURE(cont);
    cont->stretch = stretch;
    UpdateContainerRectTransforms();
}

void UIHorizontalLayout::SetStretch(UIGameObject *gameObject, float stretch)
{
    SetStretch( GetIndexFromUIGameObject(gameObject), stretch );
}

UIGameObject*
UIHorizontalLayout::GetGameObjectAt(int index)
{
    auto it = m_containers.Begin(); std::advance(it, index);
    return *it;
}

UIHorizontalLayout::UIContainer*
UIHorizontalLayout::GetContainerAt(int index)
noexcept
{
    if (index < 0 || index >= m_containers.Size())
    {
        Debug_Warn("Index " << index << " out of range.");
        return nullptr;
    }

    auto it = m_containers.Begin(); std::advance(it, index);
    return *it;
}

int
UIHorizontalLayout::GetIndexFromUIGameObject(UIGameObject *gameObject)
const noexcept
{
    int index = 0;
    for (UIContainer *cont : m_containers)
    {
        if (cont->GetGameObject() == gameObject) { return index; }
        ++index;
    }

    Debug_Warn("GameObject " << gameObject << " in UIHorizontalLayout was not "
               "found.");
    return -1;
}

void UIHorizontalLayout::UpdateContainerRectTransforms()
{
    float totalStretch = 0.0f;
    for (UIContainer *cont : m_containers) { totalStretch += cont->stretch; }

    float latestAnchorX = -1.0f;
    const float balancedWidth = (1.0f / m_containers.Size());
    for (UIContainer *cont : m_containers)
    {
        float width = totalStretch > 0 ? cont->stretch / totalStretch :
                                         balancedWidth;
        width *= 2.0f;

        RectTransform *rt = cont->rectTransform;
        rt->SetAnchorMin( Vector2(latestAnchorX, -1.0f) );
        rt->SetAnchorMax( Vector2(latestAnchorX + width, 1.0f) );

        latestAnchorX += width;
    }
}

UIHorizontalLayout::UIContainer::UIContainer(const String &name)
    : UIGameObject(name)
{
}

UIGameObject*
UIHorizontalLayout::UIContainer::GetGameObject() noexcept
{
    return SCAST<UIGameObject*>(GetChildren().Front());
}
