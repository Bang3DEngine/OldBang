#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <functional>

#include "Bang/Collection.h"

template <class T>
class Array
{
public:
    using Iterator = typename std::vector<T>::iterator;
    using RIterator = typename std::vector<T>::reverse_iterator;
    using Const_Iterator = typename std::vector<T>::const_iterator;
    using Const_RIterator = typename std::vector<T>::const_reverse_iterator;

    Array() { }
    Array(const std::vector<T> &v)      : m_vector(v) { }
    Array(int size)                     : m_vector(size) { }
    Array(int size, const T& initValue) : m_vector(size, initValue) { }
    Array(std::initializer_list<T> l)   : m_vector(l) { }

    template <class OtherIterator>
    Array(OtherIterator begin, OtherIterator end) : m_vector(begin, end) {}

    template < template<class> class OtherContainer>
    Array(const OtherContainer<T> &container)
        : Array(container.begin(), container.end()) {}

    template <class OtherIterator>
    Array(const CollectionRange<OtherIterator> &col)
        : Array(col.Begin(), col.End()) {}

    void Add(const T& x) { PushBack(x); }
    void PushBack(const T& x)  { m_vector.push_back(x);  }

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

    const T* Data() const { return m_vector.data(); }

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
        for (auto it = m_vector.RBegin(); it != m_vector.REnd(); ++it)
        {
            if (*it == x)
            {
                Iterator res = it.base();
                std::advance(res, -1);
                return res;
            }
        }
        return m_vector.End();
    }

    bool Contains(const T &x) const
    {
        return Collection::Contains(GetRangeAll(), x);
    }

    const T& Front() const { return m_vector.front(); }
    const T& Back() const  { return m_vector.back(); }
    T& Front() { return m_vector.front(); }
    T& Back()  { return m_vector.back(); }

    template < template <class> class OtherContainerClass, class OtherT = T >
    OtherContainerClass<OtherT> To() const
    {
        OtherContainerClass<OtherT> cont;
        for (const T &x : *this) { cont.Add( OtherT(x) ); }
        return cont;
    }

    Iterator Remove(const Iterator &first, const Iterator &last)
    {
        return m_vector.erase(first, last);
    }
    Iterator Remove(Iterator it) { return m_vector.erase(it); }
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
        m_vector.pop_back();
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

    void Resize(int n)   { m_vector.resize(n); }
    uint Size() const    { return m_vector.size(); }
    void Clear()         { m_vector.clear(); }
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

    T& At(std::size_t i) { return m_vector.at(i); }
    const T& At(std::size_t i) const { return m_vector.at(i); }
    T& operator[](std::size_t i) { return m_vector[i]; }
    const T& operator[](std::size_t i) const { return m_vector[i]; }

    Iterator Begin() { return m_vector.begin(); }
    Iterator End() { return m_vector.end();   }
    Const_Iterator Begin() const { return m_vector.cbegin(); }
    Const_Iterator End() const { return m_vector.cend(); }
    RIterator RBegin() { return m_vector.rbegin(); }
    RIterator REnd() { return m_vector.rend(); }
    Const_RIterator RBegin() const { return m_vector.crbegin(); }
    Const_RIterator REnd() const { return m_vector.crend(); }

    // To allow range-based for loops
    Iterator begin() { return m_vector.begin(); }
    Iterator end() { return m_vector.end(); }
    Const_Iterator begin() const { return m_vector.cbegin(); }
    Const_Iterator end() const { return m_vector.cend(); }

private:
    std::vector<T> m_vector;
};

#endif // ARRAY_H
