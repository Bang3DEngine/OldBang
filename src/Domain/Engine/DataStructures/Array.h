#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <functional>

#include "Bang/IToString.h"
#include "Bang/Collection.h"

template <class T>
class Array  : public IToString,
               public std::vector<T>
{
    typedef typename std::vector<T>::iterator               Iterator;
    typedef typename std::vector<T>::const_iterator         Const_Iterator;
    typedef typename std::vector<T>::reverse_iterator       Reverse_Iterator;
    typedef typename std::vector<T>::const_reverse_iterator Const_Reverse_Iterator;

public:
    Array() { }
    Array(const std::vector<T> &v)      : std::vector<T>(v) { }
    Array(int size)                     : std::vector<T>(size) { }
    Array(int size, const T& initValue) : std::vector<T>(size, initValue) { }
    Array(std::initializer_list<T> l)   : std::vector<T>(l) { }

    template <class OtherIterator>
    Array(OtherIterator begin, OtherIterator end)
        : std::vector<T>(begin, end) {}

    template < template<class> class OtherContainer>
    Array(const OtherContainer<T> &container)
        : Array(container.begin(), container.end()) {}

    template <class OtherIterator>
    Array(const CollectionRange<OtherIterator> &col)
        : Array(col.Begin(), col.End()) {}

    virtual ~Array() {}



    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end();   }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    Reverse_Iterator RBegin() { return this->rbegin(); }
    Reverse_Iterator REnd() { return this->rend(); }
    Const_Reverse_Iterator RBegin() const { return this->rbegin(); }
    Const_Reverse_Iterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::vector<T>::begin(); }
    Iterator end() { return this->std::vector<T>::end(); }
    Const_Iterator begin() const { return this->std::vector<T>::begin(); }
    Const_Iterator end() const { return this->std::vector<T>::end(); }

    void Add(const T& x) { this->PushBack(x); }
    void PushBack(const T& x)  { this->push_back(x);  }

    template <template <class> class OtherCollection>
    void Add(const OtherCollection<T> &otherCol)
    {
        Add (otherCol.GetRangeAll());
    }

    template <class Iterator>
    void Add(const CollectionRange<Iterator>& col)
    {
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            this->Add(*it);
        }
    }

    const T* Data() const { return this->data(); }

    bool Any(std::function< bool(const T&) > boolPredicate) const
    {
        return Collection::Any(GetRangeAll(), boolPredicate);
    }
    bool All(std::function< bool(const T&) > boolPredicate) const
    {
        return Collection::All(GetRangeAll(), boolPredicate);
    }
    bool None(std::function< bool(const T&) > boolPredicate) const
    {
        return Collection::None(GetRangeAll(), boolPredicate);
    }

    Const_Iterator Find(const T& x) const
    {
        return Collection::Find(this->GetRangeAll(), x);
    }

    Iterator Find(const T& x)
    {
        return Collection::Find(this->GetRangeAll(), x);
    }

    Iterator FindLast(const T& x)
    {
        for (auto it = this->RBegin(); it != this->REnd(); ++it)
        {
            if (*it == x)
            {
                Iterator res = it.base();
                std::advance(res, -1);
                return res;
            }
        }
        return this->End();
    }

    bool Contains(const T &x) const
    {
        return Collection::Contains(GetRangeAll(), x);
    }

    const T& Front() const { return this->front(); }
    const T& Back() const  { return this->back(); }
    T& Front() { return this->front(); }
    T& Back()  { return this->back(); }

    template < template <class> class OtherContainerClass, class OtherT = T >
    OtherContainerClass<OtherT> To() const
    {
        OtherContainerClass<OtherT> cont;
        for (const T &x : *this) { cont.Add( OtherT(x) ); }
        return cont;
    }

    Iterator Remove(const Iterator &first, const Iterator &last)
    {
        return this->erase(first, last);
    }
    Iterator Remove(Iterator it) { return this->Remove(it, it); }
    Iterator Remove(const T& x)
    {
        Iterator it = this->Find(x);
        if (it != this->End()) { return this->Remove(it); }
        return this->End();
    }
    void RemoveAll(const T& x)
    {
        for (Iterator it = this->Begin(); it != this->End(); ++it)
        {
            if (*it == x)
            {
                it = this->Remove(it);
                --it;
            }
        }
    }

    T& PopBack()
    {
        T& back = Back();
        this->pop_back();
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

    uint Count(std::function< bool(const T&) > boolPredicate) const
    {
        return Collection::Count(GetRangeAll(), boolPredicate);
    }
    uint Count(const T& x) const
    {
        return Collection::Count(GetRangeAll(), x);
    }

    void Resize(int n)   { this->resize(n); }
    uint Size() const    { return this->size(); }
    void Clear()         { this->clear(); }
    bool IsEmpty() const { return this->Size() == 0; }

    CollectionRange<Iterator> GetRangeAll()
    {
        return CollectionRange<Iterator>(this->Begin(), this->End());
    }
    CollectionRange<Const_Iterator> GetRangeAll() const
    {
        return CollectionRange<Const_Iterator>(this->Begin(), this->End());
    }

    virtual String ToString() const override
    {
        return Collection::ToString(GetRangeAll(), "[", "]");
    }

    operator CollectionRange<Iterator>() { return GetRangeAll(); }
    operator CollectionRange<Iterator>() const { return GetRangeAll(); }

    T& operator[](int i)
    {
        return std::vector<T>::operator [](i);
    }
    const T& operator[](int i) const
    {
        return std::vector<T>::operator [](i);
    }
};

#endif // ARRAY_H
