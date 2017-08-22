#include "Bang/GUIDirLayout.h"

#include "Bang/Array.h"
#include "Bang/RectTransform.h"

GUIDirLayout::GUIDirLayout() : GUIDirLayout("GUIDirLayout", false)
{
}

GUIDirLayout::GUIDirLayout(const String &name, bool vertical) :
    UIGameObject(name),
    m_vertical(vertical)
{
}

GUIDirLayout::~GUIDirLayout()
{
}

void GUIDirLayout::Add(UIGameObject *gameObject, int _index)
{
    UIContainer *cont = new UIContainer(gameObject);
    m_childrenContainers.Add(gameObject, cont);
    int index = (_index != -1 ? _index : GetChildren().Size());
    cont->SetParent(this, index);
    UpdateChildrenRectTransforms();
}

void GUIDirLayout::Move(int indexFrom, int indexTo)
{
    UIGameObject *uigo = Take(indexFrom);
    Add(uigo, indexTo);
}

UIGameObject* GUIDirLayout::Take(int index)
{
    if (index < 0 || index >= GetChildren().Size())
    {
        Debug_Warn("Index " << index << " out of range.");
        return nullptr;
    }

    UIGameObject *uigo = SCAST<UIGameObject*>( GetChild(index)->GetChild(0) );
    return Take(uigo);
}

UIGameObject* GUIDirLayout::Take(UIGameObject *gameObject)
{
    gameObject->SetParent(nullptr);
    m_childrenContainers.Remove(gameObject);
    UpdateChildrenRectTransforms();
    return gameObject;
}

void GUIDirLayout::SetSpacing(int spacingPx)
{
    m_spacingPx = spacingPx;
    UpdateChildrenRectTransforms();
}

void GUIDirLayout::SetStretch(int index, float stretch)
{
    UIContainer *cont = SCAST<UIContainer*>( GetChild(index) );
    cont->stretch = stretch;
    UpdateChildrenRectTransforms();
}

void GUIDirLayout::SetStretch(UIGameObject *gameObject, float stretch)
{
    SetStretch( GetIndexFromChild(gameObject), stretch );
}

int
GUIDirLayout::GetIndexFromChild(UIGameObject *gameObject)
const noexcept
{
    return GetChildren().IndexOf( m_childrenContainers.At(gameObject) );
}

void GUIDirLayout::UpdateChildrenRectTransforms()
{
    const int numChildren = GetChildren().Size();
    float latestAnchor = (m_vertical ? 1.0f : -1.0f);
    const float balancedSize = (1.0f / numChildren);

    float totalStretch = 0.0f;
    for (auto it : m_childrenContainers) { totalStretch += it.second->stretch; }

    int i = 0;
    for (GameObject *_cont : GetChildren())
    {
        const UIContainer *cont = SCAST<UIContainer*>(_cont);

        float sizeNDC;
        if (totalStretch > 0) { sizeNDC  = (cont->stretch / totalStretch); }
        else { sizeNDC = balancedSize; }
        sizeNDC *= 2.0f;

        const bool first = (i == 0);
        const bool last  = (i == numChildren-1);
        const int spacing = (first || last) ? m_spacingPx / 1.5 : m_spacingPx;
        RectTransform *rtCont = cont->rectTransform;
        int marginLeft = 0, marginTop = 0, marginRight = 0, marginBot = 0;
        if (m_vertical)
        {
            rtCont->SetAnchorMin( Vector2(-1.0f, latestAnchor - sizeNDC) );
            rtCont->SetAnchorMax( Vector2(1.0f, latestAnchor) );
            marginTop = last ? spacing : ((spacing+1) / 2);
            marginBot = first ? spacing : (spacing / 2);
        }
        else
        {
            rtCont->SetAnchorMin( Vector2(latestAnchor, -1.0f) );
            rtCont->SetAnchorMax( Vector2(latestAnchor + sizeNDC, 1.0f) );
            marginLeft  = last ? spacing : ((spacing+1) / 2);
            marginRight = first ? spacing : (spacing / 2);
        }
        rtCont->SetMargins(marginLeft, marginTop, marginRight, marginBot);

        latestAnchor += sizeNDC * (m_vertical ? -1.0f : 1.0f);
        ++i;
    }
}
