#ifndef LIST_H
#define LIST_H

#include <list>
#include <functional>

#include "Bang/Containers.h"

NAMESPACE_BANG_BEGIN

template <class T>
class List
{
public:
    using Iterator = typename std::list<T>::iterator;
    using RIterator = typename std::list<T>::reverse_iterator;
    using Const_Iterator = typename std::list<T>::const_iterator;
    using Const_RIterator = typename std::list<T>::const_reverse_iterator;

    List();
    List(const std::list<T> &l);
    List(int size);
    List(int size, const T& initValue);
    List(std::initializer_list<T> l);

    template <class OtherIterator>
    List(OtherIterator begin, OtherIterator end);

    void Splice(Iterator insertAfter, List<T> &from);
    void Splice(Iterator insertAfter, List<T> &from, Iterator element);
    void Splice(Iterator insertAfter, List<T> &src,
                Iterator srcRangeBegin, Iterator srcRangeEnd);

    Iterator Insert(const Iterator& it, const T& x);

    Iterator Insert(int index, const T& x);

    Iterator Insert(const Iterator& first, int count, const T& x);

    void PushFront(const T& x);
    void PopFront();

    List<T> Concat(const List<T> &l) const;

    void Sort();
    template<class StrictWeakOrdering>
    void Sort(const StrictWeakOrdering &sortClass);

    void PushBack(const T& x);
    template <template <class OtherT> class Container, class OtherT>
    void PushBack(const Container<OtherT> &container);

    const T* Data() const;

    Const_Iterator Find(const T& x) const;
    Iterator Find(const T& x);
    Iterator FindLast(const T& x);
    bool Contains(const T &x) const;

    const T& Front() const;
    const T& Back() const;
    T& Front();
    T& Back();

    Iterator Remove(const Iterator &first, const Iterator &last);
    Iterator Remove(Iterator it);
    Iterator Remove(const T& x);
    void RemoveAll(const T& x);

    void PopBack();
    int IndexOf(const T& x) const;

    void Resize(int n);
    uint Size() const;
    void Clear();
    bool IsEmpty() const;

    Iterator Begin();
    Iterator End();
    Const_Iterator Begin() const;
    Const_Iterator End() const;
    RIterator RBegin();
    RIterator REnd();
    Const_RIterator RBegin() const;
    Const_RIterator REnd() const;

    // To allow range-based for loops
    Iterator begin();
    Iterator end();
    Const_Iterator begin() const;
    Const_Iterator end() const;

    template < template <class> class Container, class OtherT = T >
    Container<OtherT> To() const;

private:
    std::list<T> m_list;
};

NAMESPACE_BANG_END

#include "List.tcc"

#endif // LIST_H
