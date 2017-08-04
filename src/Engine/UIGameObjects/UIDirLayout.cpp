#include "Bang/UIDirLayout.h"

#include "Bang/Array.h"
#include "Bang/RectTransform.h"

UIDirLayout::UIDirLayout(bool vertical) :
    m_vertical(vertical)
{
}

UIDirLayout::~UIDirLayout()
{
}


#include "Bang/Input.h"
#include "Bang/Random.h"
#include "Bang/UIImage.h"
#include "Bang/UIInputText.h"
void UIDirLayout::OnUpdate()
{
    if (Input::GetKeyDown(Input::Key::A))
    {
        UIGameObject *uig = new UIGameObject();
        UIInputText *text = new UIInputText();
        text->GetBackgroundImage()->SetTint(Random::GetColorOpaque());
        text->SetParent(uig);
        Add(uig, 0);
    }
    else if (Input::GetKeyDown(Input::Key::R))
    {
        Take(1);
    }
    else if (Input::GetKeyDown(Input::Key::M))
    {
        Move(0, GetChildren().Size()-1);
    }
}

void UIDirLayout::Add(UIGameObject *gameObject, int _index)
{
    m_childrenInfo.Set(gameObject, ChildInfo());
    int index = (_index != -1 ? _index : GetChildren().Size());
    gameObject->SetParent(this, index);
    UpdateChildrenRectTransforms();
}

void UIDirLayout::Move(int indexFrom, int indexTo)
{
    UIGameObject *uigo = Take(indexFrom);
    Add(uigo, indexTo);
}

UIGameObject* UIDirLayout::Take(int index)
{
    if (index < 0 || index >= GetChildren().Size())
    {
        Debug_Warn("Index " << index << " out of range.");
        return nullptr;
    }

    UIGameObject *uigo = SCAST<UIGameObject*>( GetChild(index) );
    return Take(uigo);
}

UIGameObject* UIDirLayout::Take(UIGameObject *gameObject)
{
    gameObject->SetParent(nullptr);
    m_childrenInfo.Remove(gameObject);
    UpdateChildrenRectTransforms();
    return gameObject;
}

void UIDirLayout::SetStretch(int index, float stretch)
{
    GetChildInfoAt(index)->stretch = stretch;
    UpdateChildrenRectTransforms();
}

void UIDirLayout::SetStretch(UIGameObject *gameObject, float stretch)
{
    SetStretch( GetIndexFromChild(gameObject), stretch );
}

int
UIDirLayout::GetIndexFromChild(UIGameObject *gameObject)
const noexcept
{
    return GetChildren().IndexOf(gameObject);
}

UIDirLayout::ChildInfo*
UIDirLayout::GetChildInfoAt(int index)
{
    return &( m_childrenInfo[ SCAST<UIGameObject*>( GetChild(index) ) ] );
}

void UIDirLayout::UpdateChildrenRectTransforms()
{
    float totalStretch = 0.0f;
    for (auto it : m_childrenInfo) { totalStretch += it.second.stretch; }

    float latestAnchor = -1.0f;
    const float balancedSize = (1.0f / GetChildren().Size());
    for (GameObject *go : GetChildren())
    {
        UIGameObject *uigo = SCAST<UIGameObject*>(go);
        const ChildInfo &childInfo = m_childrenInfo[uigo];
        float size =(totalStretch > 0 ? (childInfo.stretch / totalStretch) :
                                         balancedSize);
        size *= 2.0f;

        RectTransform *rt = uigo->rectTransform;
        Vector2 anchorMin(latestAnchor, -1.0f);
        Vector2 anchorMax(latestAnchor + size, 1.0f);
        if (m_vertical)
        {
            anchorMin = Vector2(anchorMin.y, anchorMin.x);
            anchorMax = Vector2(anchorMax.y, anchorMax.x);
        }

        rt->SetAnchorMin(anchorMin);
        rt->SetAnchorMax(anchorMax);

        latestAnchor += size;
    }
}
