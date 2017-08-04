#ifndef LIST_H
#define LIST_H

#include <list>
#include <functional>

#include "Bang/Debug.h"
#include "Bang/Collection.h"

template <class T>
class List
{
public:
    using Iterator = typename std::list<T>::iterator;
    using RIterator = typename std::list<T>::reverse_iterator;
    using Const_Iterator = typename std::list<T>::const_iterator;
    using Const_RIterator = typename std::list<T>::const_reverse_iterator;

    List() { }
    List(const std::list<T> &l)        : m_list(l) { }
    List(int size)                     : m_list(size) { }
    List(int size, const T& initValue) : m_list(size, initValue) { }
    List(std::initializer_list<T> l)   : m_list(l) { }

    template <class OtherIterator>
    List(OtherIterator begin, OtherIterator end) : m_list(begin, end) {}

    template <class OtherIterator>
    List(const CollectionRange<OtherIterator> &col)
        : List(col.Begin(), col.End()) {}

    void Splice(Iterator insertAfter, List<T> &from)
    {
        m_list.splice(insertAfter, from.m_list);
    }

    void Splice(Iterator insertAfter, List<T> &from, Iterator element)
    {
        m_list.splice(insertAfter, from.m_list, element);
    }

    void Splice(Iterator insertAfter, List<T> &src,
                Iterator srcRangeBegin, Iterator srcRangeEnd)
    {
        m_list.splice(insertAfter, src.m_list, srcRangeBegin, srcRangeEnd);
    }

    void Insert(const Iterator& it, const T& x)
    {
        m_list.insert(it, x);
    }

    void Insert(int index, const T& x)
    {
        ASSERT(index >= 0 && index <= Size());
        auto it = Begin(); std::advance(it, index);
        Insert(it, x);
    }

    void Insert(const Iterator& first, int count, const T& x)
    {
        m_list.insert(first, count, x);
    }

    void PushFront(const T& x) { m_list.push_front(x); }
    const T& PopFront()
    {
        const T& front = Front();
        m_list.pop_front();
        return front;
    }

    List<T> Concat(const List<T> &l) const
    {
        List<T> result = *this;
        for(const T& x : l) { result.PushBack(x); }
        return result;
    }

    void Sort()
    {
        m_list.sort();
    }

    template<class StrictWeakOrdering>
    void Sort(const StrictWeakOrdering &sortClass)
    {
        m_list.sort(sortClass);
    }

    void Add(const T& x) { PushBack(x); }
    void PushBack(const T& x)  { m_list.push_back(x);  }

    template <template <class> class OtherCollection>
    void Add(const OtherCollection<T> &otherCol)
    {
        Add (otherCol.GetRangeAll());
    }

    template <class Iterator>
    void Add(const CollectionRange<Iterator>& col)
    {
        for (Iterator it = col.Begin(); it != col.End(); ++it) { Add(*it); }
    }

    const T* Data() const { return m_list.data(); }

    Const_Iterator Find(const T& x) const
    {
        return Collection::Find(GetRangeAll(), x);
    }

    Iterator Find(const T& x)
    {
        return Collection::Find(GetRangeAll(), x);
    }

    Iterator FindLast(const T& x)
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

    bool Contains(const T &x) const { return Find(x) != End(); }

    const T& Front() const { return m_list.front(); }
    const T& Back() const  { return m_list.back(); }
    T& Front() { return m_list.front(); }
    T& Back()  { return m_list.back(); }

    template < template <class> class OtherContainerClass, class OtherT = T >
    OtherContainerClass<OtherT> To() const
    {
        OtherContainerClass<OtherT> cont;
        for (const T &x : *this) { cont.Add( OtherT(x) ); }
        return cont;
    }

    Iterator Remove(const Iterator &first, const Iterator &last)
    {
        return m_list.erase(first, last);
    }
    Iterator Remove(Iterator it) { return m_list.erase(it); }
    Iterator Remove(const T& x)
    {
        Iterator it = Find(x);
        if (it != End()) { return Remove(it); }
        return End();
    }
    void RemoveAll(const T& x)
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            if (*it == x)
            {
                it = Remove(it);
                --it;
            }
        }
    }

    T& PopBack()
    {
        T& back = Back();
        m_list.pop_back();
        return back;
    }

    int IndexOf(const T& x) const
    {
        int i = 0;
        for (const T& y : *this)
        {
            if (x == y)  { return i; }
            ++i;
        }
        return -1;
    }

    void Resize(int n) { m_list.resize(n); }
    uint Size() const  { return m_list.size(); }
    void Clear()       { m_list.clear(); }
    bool IsEmpty() const { return Size() == 0; }

    CollectionRange<Iterator> GetRangeAll()
    {
        return CollectionRange<Iterator>(Begin(), End());
    }
    CollectionRange<Const_Iterator> GetRangeAll() const
    {
        return CollectionRange<Const_Iterator>(Begin(), End());
    }

    operator CollectionRange<Iterator>() { return GetRangeAll(); }
    operator CollectionRange<Iterator>() const { return GetRangeAll(); }


    Iterator Begin() { return m_list.begin(); }
    Iterator End() { return m_list.end();   }
    Const_Iterator Begin() const { return m_list.cbegin(); }
    Const_Iterator End() const { return m_list.cend(); }
    RIterator RBegin() { return m_list.rbegin(); }
    RIterator REnd() { return m_list.rend(); }
    Const_RIterator RBegin() const { return m_list.crbegin(); }
    Const_RIterator REnd() const { return m_list.crend(); }

    // To allow range-based for loops
    Iterator begin() { return m_list.begin(); }
    Iterator end() { return m_list.end(); }
    Const_Iterator begin() const { return m_list.cbegin(); }
    Const_Iterator end() const { return m_list.cend(); }

private:
    std::list<T> m_list;
};

#endif // LIST_H
