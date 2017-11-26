#ifndef GAMEOBJECT_TCC
#define GAMEOBJECT_TCC

#include "Bang/GameObject.h"
#include "Bang/ComponentFactory.h"

NAMESPACE_BANG_BEGIN

template <class T, class... Args>
T* GameObject::Create(Args... args)
{
    T* gameObject = ObjectManager::Create<T>(args...);
    return gameObject;
}

template <class T>
T* GameObject::AddComponent(int index)
{
    T *c = ComponentFactory::Create<T>();
    this->AddComponent(c, index);
    return c;
}

template <class T>
T* GameObject::GetComponent() const
{
    for (Component *comp : m_components)
    {
        T *tp = Cast<T*>(comp);
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
        T *tp = Cast<T*>(comp);
        if (tp) { comps_l.PushBack(tp); }
    }
    return comps_l;
}

template <class T>
T* GameObject::GetComponentInParent(bool recursive) const
{
    if (!GetParent()) { return nullptr; }
    T* comp = GetParent()->GetComponent<T>();
    if (comp) { return comp; }
    return recursive ? GetParent()->GetComponentInParent<T>() : nullptr;
}

template <class T>
List<T*> GameObject::GetComponentsInParent(bool recursive) const
{
    List<T*> result;
    if (!GetParent()) { return result; }
    result = GetParent()->GetComponents<T>();
    return recursive ? result.Concat(GetParent()->GetComponentsInParent<T>()) :
                       result;
}

template <class T>
T* GameObject::GetComponentInChildren(bool recursive) const
{
    T *compThis = GetComponent<T>();
    if (compThis) { return compThis; }
    return GetComponentInChildrenOnly<T>(recursive);
}

template <class T>
List<T*> GameObject::GetComponentsInChildren(bool recursive) const
{
    List<T*> comps_l;
    comps_l.PushBack( GetComponents<T>() );
    comps_l.PushBack( GetComponentsInChildrenOnly<T>(recursive) );
    return comps_l;
}

template<class T>
T *GameObject::GetComponentInChildrenOnly(bool recursive) const
{
    for (auto c = GetChildren().Begin(); c != GetChildren().End(); ++c)
    {
        T *comp = (*c)->GetComponent<T>();
        if (comp) return comp;
        if (recursive)
        {
            comp = (*c)->GetComponentInChildren<T>(true);
            if (comp) return comp;
        }
    }
    return nullptr;
}

template<class T>
List<T*> GameObject::GetComponentsInChildrenOnly(bool recursive) const
{
    List<T*> comps;
    for (auto c = GetChildren().Begin(); c != GetChildren().End(); ++c)
    {
        List<T*> childChildrenComps =
                recursive ? (*c)->GetComponentsInChildren<T>() :
                            (*c)->GetComponents<T>();
        comps.Splice(comps.End(), childChildrenComps); //concat
    }
    return comps;
}

template <class T>
bool GameObject::HasComponent() const { return GetComponent<T>() ; }

template <class T>
int GameObject::CountComponents() const
{
    int count = 0;
    for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
    {
        T *tp = Cast<T*>(comp);
        if (tp) { ++count; }
    }
    return count;
}


template <class T>
void GameObject::RemoveComponent()
{
    for (Component *comp : m_components)
    {
        T *compT = Cast<T*>(comp);
        if (compT)
        {
            this->RemoveComponent(compT);
            break;
        }
    }
}

NAMESPACE_BANG_END

#endif // GAMEOBJECT_TCC
