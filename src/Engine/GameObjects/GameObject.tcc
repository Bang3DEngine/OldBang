#ifndef GAMEOBJECT_TCC
#define GAMEOBJECT_TCC

#include "Bang/GameObject.h"
#include "Bang/ComponentFactory.h"

template <class T>
T* GameObject::AddComponent(int index)
{
    T *c = ComponentFactory::CreateComponent<T>();
    this->AddComponent(c, index);
    return c;
}

template <class T>
T* GameObject::GetComponent() const
{
    for (Component *comp : m_components)
    {
        T *tp = DCAST<T*>(comp);
        if (tp) { return tp; }
    }
    return nullptr;
}

template <class T>
List<T*> GameObject::GetComponents() const
{
    List<T*> comps_l;
    for (Component *comp : m_components)
    {
        T *tp = DCAST<T*>(comp);
        if (tp) { comps_l.PushBack(tp); }
    }
    return comps_l;
}

template <class T>
T* GameObject::GetComponentInParent() const
{
    if (!parent) { return nullptr; }
    T* comp = parent->GetComponent<T>();
    if (comp) { return comp; }
    return parent->GetComponentInParent<T>();
}

template <class T>
List<T*> GameObject::GetComponentsInParent() const
{
    List<T*> result;
    if (!parent) { return result; }
    result = parent->GetComponents<T>();
    return result.Concat(parent->GetComponentsInParent<T>());
}

template <class T>
T* GameObject::GetComponentInChildren() const
{
    for (auto c = GetChildren().Begin(); c != GetChildren().End(); ++c)
    {
        T *comp = (*c)->GetComponent<T>();
        if (comp) return comp;
        comp = (*c)->GetComponentInChildren<T>();
        if (comp) return comp;
    }
    return nullptr;
}

template <class T>
List<T*> GameObject::GetComponentsInChildren() const
{
    List<T*> comps_l;
    for (auto c = GetChildren().Begin(); c != GetChildren().End(); ++c)
    {
        List<T*> childComps = (*c)->GetComponents<T>();
        comps_l.Splice(comps_l.End(), childComps); //concat
        List<T*> childChildrenComps = (*c)->GetComponentsInChildren<T>();
        comps_l.Splice(comps_l.End(), childChildrenComps); //concat
    }
    return comps_l;
}

template <class T>
bool GameObject::HasComponent() const { return GetComponent<T>() ; }

template <class T>
int GameObject::CountComponents() const
{
    int count = 0;
    for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
    {
        T *tp = DCAST<T*>(comp);
        if (tp) { ++count; }
    }
    return count;
}


template <class T>
void GameObject::RemoveComponent()
{
    for (Component *comp : m_components)
    {
        T *compT = DCAST<T*>(comp);
        if (compT)
        {
            this->RemoveComponent(compT);
            break;
        }
    }
}

#endif // GAMEOBJECT_TCC
