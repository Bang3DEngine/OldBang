#ifndef TREE_H
#define TREE_H

#include "Bang/List.h"

NAMESPACE_BANG_BEGIN

template<class T>
class Tree
{
public:
    Tree();
    ~Tree();

    Tree<T>* AddChild();
    Tree<T>* AddChild(const T& data);
    Tree<T>* AddChild(const T& data, uint index);

    void SetData(const T &data);
    void SetParent(Tree<T> *parentTree);
    void SetParent(Tree<T> *parentTree, uint index);

    void Clear();

    int GetDepth() const;
    T& GetData();
    const T& GetData() const;
    Tree<T>* GetParent() const;
    List< Tree<T>* >& GetChildren();
    const List< Tree<T>* >& GetChildren() const;
    List< Tree<T>* > GetChildrenRecursive() const;

private:
    T m_data;
    Tree<T> *p_parent = nullptr;
    List< Tree<T>* > m_subTrees;
};

NAMESPACE_BANG_END

#include "Tree.tcc"

#endif // TREE_H
