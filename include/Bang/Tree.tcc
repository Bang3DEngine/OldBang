#ifndef TREE_TCC
#define TREE_TCC

#include "Bang/Tree.h"

NAMESPACE_BANG_BEGIN

template<class T>
Tree<T>::Tree()
{
}

template<class T>
Tree<T>::~Tree()
{
    SetParent(nullptr);
    Clear();
}

template<class T>
Tree<T>* Tree<T>::AddChild()
{
    Tree<T> *childTree = new Tree<T>();
    childTree->SetParent(this);
    return childTree;
}

template<class T>
Tree<T>* Tree<T>::AddChild(const T &data)
{
    Tree<T> *child = AddChild();
    child->SetData(data);
    return child;
}

template<class T>
void Tree<T>::SetData(const T &data)
{
    m_data = data;
}

template<class T>
void Tree<T>::SetParent(Tree<T> *parentTree)
{
    ENSURE (GetParent() != parentTree);

    if (GetParent())
    {
        GetParent()->m_subTrees.Remove(this);
    }

    p_parent = parentTree;
    if (GetParent())
    {
        GetParent()->m_subTrees.PushBack(this);
    }
}

template<class T>
void Tree<T>::Clear()
{
    while (!m_subTrees.IsEmpty())
    {
        Tree<T> *child = m_subTrees.Front();
        m_subTrees.PopFront();
        delete child;
    }
    m_subTrees.Clear();
}

template<class T>
int Tree<T>::GetDepth() const
{
    return GetParent() ? (GetParent()->GetDepth() + 1) : 0;
}

template<class T>
const List< Tree<T>* > &Tree<T>::GetChildren() const { return m_subTrees; }

template<class T>
List< Tree<T>* > Tree<T>::GetChildrenRecursive() const
{
    List< Tree<T>* > result = GetChildren();
    for (Tree<T>* child : GetChildren())
    {
        result.PushBack(child->GetChildrenRecursive());
    }
    return result;
}

template<class T>
Tree<T>* Tree<T>::GetParent() const { return p_parent; }

template<class T>
const T& Tree<T>::GetData() const { return m_data; }

NAMESPACE_BANG_END

#endif // TREE_TCC
