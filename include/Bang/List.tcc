#ifndef LIST_TCC
#define LIST_TCC

#include "Bang/List.h"

NAMESPACE_BANG_BEGIN

template<class T>
List<T>::List() {}

template<class T>
List<T>::List(const std::list<T> &l) : m_list(l) {}

template<class T>
List<T>::List(int size) : m_list(size) {}

template<class T>
List<T>::List(int size, const T &initValue) : m_list(size, initValue) {}

template<class T>
List<T>::List(std::initializer_list<T> l) : m_list(l) {}

template<class T>
template <class OtherIterator>
List<T>::List(OtherIterator begin, OtherIterator end) : m_list(begin, end) {}

template<class T>
void List<T>::Splice(typename List<T>::Iterator insertAfter,
                     List<T> &from)
{
    m_list.splice(insertAfter, from.m_list);
}

template<class T>
void List<T>::Splice(typename List<T>::Iterator insertAfter,
                     List<T> &from,
                     typename List<T>::Iterator element)
{
    m_list.splice(insertAfter, from.m_list, element);
}

template<class T>
void List<T>::Splice(typename List<T>::Iterator insertAfter,
                     List<T> &src,
                     typename List<T>::Iterator srcRangeBegin,
                     typename List<T>::Iterator srcRangeEnd)
{
    m_list.splice(insertAfter, src.m_list, srcRangeBegin, srcRangeEnd);
}

template<class T>
typename List<T>::Iterator
List<T>::Insert(const typename List<T>::Iterator &it, const T &x)
{
    return m_list.insert(it, x);
}

template<class T>
typename List<T>::Iterator
List<T>::Insert(int index, const T &x)
{
    ASSERT(index >= 0 && index <= Size());
    auto it = Begin(); std::advance(it, index);
    return Insert(it, x);
}

template<class T>
typename List<T>::Iterator
List<T>::Insert(const typename List<T>::Iterator &first, int count, const T &x)
{
    return m_list.insert(first, count, x);
}

template<class T>
void List<T>::PushFront(const T &x) { m_list.push_front(x); }

template<class T>
void List<T>::PopFront() { m_list.pop_front(); }

template<class T>
List<T> List<T>::Concat(const List<T> &l) const
{
    List<T> result = *this;
    for(const T& x : l) { result.PushBack(x); }
    return result;
}

template<class T>
void List<T>::Sort()
{
    m_list.sort();
}

template<class T>
void List<T>::PushBack(const T &x)  { m_list.push_back(x);  }

template<class T>
template <template <class OtherT> class Container, class OtherT>
void List<T>::PushBack(const Container<OtherT> &container)
{
    for (auto it = container.Begin(); it != container.End(); ++it)
    {
        PushBack(*it);
    }
}

template <class T>
template <class StrictWeakOrdering>
void List<T>::Sort(const StrictWeakOrdering &sortClass)
{
    m_list.sort(sortClass);
}

template<class T>
bool List<T>::IsEmpty() const { return Size() == 0; }

template<class T>
void List<T>::Clear() { m_list.clear(); }

template<class T>
uint List<T>::Size() const  { return m_list.size(); }

template<class T>
void List<T>::Resize(int n) { m_list.resize(n); }

template<class T>
int List<T>::IndexOf(const T &x) const
{
    int i = 0;
    for (const T& y : *this)
    {
        if (x == y)  { return i; }
        ++i;
    }
    return -1;
}

template<class T>
void List<T>::PopBack() { m_list.pop_back(); }

template<class T>
void List<T>::RemoveAll(const T &x)
{
    for (Iterator it = Begin(); it != End(); )
    {
        if (*it == x) { it = Remove(it); }
        else { ++it; }
    }
}

template<class T>
const T *List<T>::Data() const { return m_list.data(); }

template<class T>
typename List<T>::Const_Iterator List<T>::Find(const T &x) const
{
    return Containers::Find(*this, x);
}

template<class T>
typename List<T>::Iterator List<T>::Find(const T &x)
{
    return Containers::Find(*this, x);
}

template<class T>
typename List<T>::Iterator List<T>::FindLast(const T &x)
{
    for (auto it = RBegin(); it != REnd(); ++it)
    {
        if (*it == x)
        {
            Iterator res = it.base();
            std::advance(res, -1);
            return res;
        }
    }
    return End();
}

template<class T>
bool List<T>::Contains(const T &x) const { return Find(x) != End(); }

template<class T>
const T &List<T>::Front() const { return m_list.front(); }

template<class T>
const T &List<T>::Back() const  { return m_list.back(); }

template<class T>
T &List<T>::Front() { return m_list.front(); }

template<class T>
T &List<T>::Back()  { return m_list.back(); }

template<class T>
typename List<T>::Iterator
List<T>::Remove(const typename List<T>::Iterator &first,
                const typename List<T>::Iterator &last)
{
    return m_list.erase(first, last);
}

template<class T>
typename List<T>::Iterator List<T>::Remove(typename List<T>::Iterator it)
{
    return m_list.erase(it);
}

template<class T>
typename List<T>::Iterator List<T>::Remove(const T &x)
{
    Iterator it = Find(x);
    if (it != End()) { return Remove(it); }
    return End();
}

template<class T>
typename List<T>::Const_RIterator
List<T>::REnd() const { return m_list.crend(); }

template<class T>
typename List<T>::Const_RIterator
List<T>::RBegin() const { return m_list.crbegin(); }

template<class T>
typename List<T>::RIterator
List<T>::REnd() { return m_list.rend(); }

template<class T>
typename List<T>::RIterator
List<T>::RBegin() { return m_list.rbegin(); }

template<class T>
typename List<T>::Const_Iterator
List<T>::End() const { return m_list.cend(); }

template<class T>
typename List<T>::Const_Iterator
List<T>::Begin() const { return m_list.cbegin(); }

template<class T>
typename List<T>::Iterator List<T>::End() { return m_list.end();   }

template<class T>
typename List<T>::Iterator List<T>::Begin() { return m_list.begin(); }

template<class T>
typename List<T>::Iterator List<T>::begin() { return m_list.begin(); }

template<class T>
typename List<T>::Iterator List<T>::end() { return m_list.end(); }

template<class T>
typename List<T>::Const_Iterator
List<T>::begin() const { return m_list.cbegin(); }

template<class T>
typename List<T>::Const_Iterator
List<T>::end() const { return m_list.cend(); }

template <class T>
template < template <class> class Container, class OtherT>
Container<OtherT> List<T>::To() const
{
    Container<OtherT> cont;
    for (const T &x : *this) { cont.PushBack( Cast<OtherT>(x) ); }
    return cont;
}

NAMESPACE_BANG_END

#endif // LIST_TCC
