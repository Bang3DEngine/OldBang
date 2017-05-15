#ifndef LIST_H
#define LIST_H

#include <list>
#include <algorithm>
#include <functional>

#include "Bang/IToString.h"

template <class T>
class List : public IToString,
             public std::list<T>
{
    typedef typename std::list<T>::iterator               Iterator;
    typedef typename std::list<T>::const_iterator         Const_Iterator;
    typedef typename std::list<T>::reverse_iterator       Reverse_Iterator;
    typedef typename std::list<T>::const_reverse_iterator Const_Reverse_Iterator;

public:
    List() { }
    List(const std::list<T> &l)         : std::list<T>(l) { }
    List(int size)                      : std::list<T>(size) { }
    List(int size, const T& initValue)  : std::list<T>(size, initValue) { }
    List(std::initializer_list<T> l)    : std::list<T>(l) { }
    virtual ~List() {}

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end();   }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    Reverse_Iterator RBegin() { return this->rbegin(); }
    Reverse_Iterator REnd() { return this->rend(); }
    Const_Reverse_Iterator RBegin() const { return this->rbegin(); }
    Const_Reverse_Iterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::list<T>::begin(); }
    Iterator end() { return this->std::list<T>::end(); }
    Const_Iterator begin() const { return this->std::list<T>::begin(); }
    Const_Iterator end() const { return this->std::list<T>::end(); }

    void Splice(Iterator insertAfter, List<T> &from)
    {
        this->splice(insertAfter, from);
    }

    void Splice(Iterator insertAfter, List<T> &from, Iterator element)
    {
        this->splice(insertAfter, from, element);
    }

    void Splice(Iterator insertAfter, List<T> &src,
                Iterator srcRangeBegin, Iterator srcRangeEnd)
    {
        this->splice(insertAfter, src, srcRangeBegin, srcRangeEnd);
    }

    void Insert(const Iterator& it, const T& x)
    {
        this->InsertBefore(it, x);
    }

    void Insert(const Iterator& first, int count, const T& x)
    {
        this->insert(first, count, x);
    }

    void InsertBefore(const Iterator& it, const T& x)
    {
        this->insert(it, x);
    }

    void InsertAfter(const Iterator& it, const T& x)
    {
        Iterator itAfter = it;
        std::advance(itAfter, 1);
        this->insert(itAfter, x);
    }

    void PushFront(const T& x) { this->push_front(x); }
    const T& PopFront()
    {
        const T& front = this->Front();
        this->pop_front();
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
        this->sort();
    }

    template<class StrictWeakOrdering>
    void Sort(const StrictWeakOrdering &sortClass)
    {
        this->sort(sortClass);
    }

    void Add(const T& x) { this->PushBack(x); }
    void PushBack(const T& x)  { this->push_back(x);  }

    template <template <class> class OtherContainer >
    void Add(const OtherContainer<T>& collection)
    {
        for (const T &x : collection)
        {
            this->Add(x);
        }
    }

    const T* Data() const { return this->data(); }

    bool Any(std::function< bool(const T&) > boolPredicate) const
    {
        for (const auto &x : *this) { if (boolPredicate(x)) { return true; } }
        return false;
    }
    bool All(std::function< bool(const T&) > boolPredicate) const
    {
        for (const auto &x : *this) { if (!boolPredicate(x)) { return false; } }
        return true;
    }
    bool None(std::function< bool(const T&) > boolPredicate) const
    {
        return !Any(boolPredicate);
    }

    Const_Iterator Find(const T& x) const
    {
        return std::find(this->Begin(), this->End(), x);
    }

    Iterator Find(const T& x)
    {
        return std::find(this->Begin(), this->End(), x);
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

    bool Contains(const T &x) const { return this->Find(x) != this->End(); }

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

    Iterator Remove(Iterator it) { return this->erase(it); }
    Iterator Remove(const Iterator &first, const Iterator &last)
    {
        return this->erase(first, last);
    }
    Iterator Remove(const T& x)
    {
        Iterator it = this->Find(x);
        if (it != this->End()) { return this->erase(it); }
        return this->End();
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

    void RemoveAll(const T& x)
    {
        for (Iterator it = this->Begin(); it != this->End(); ++it)
        {
            if (*it == x)
            {
                it = this->erase(it);
                --it;
            }
        }
    }

    uint Count(std::function< bool(const T&) > boolPredicate) const
    {
        uint count = 0;
        for (const auto &x : *this) { if (boolPredicate(x)) { ++count; } }
        return count;
    }
    uint Count(const T& x) const
    {
        int count = 0;
        for (const auto &y : *this) { if (x == y) { ++count; } }
        return count;
    }

    void Resize(int n) { this->resize(n); }
    uint Size() const  { return this->size(); }
    void Clear()       { this->clear(); }
    bool IsEmpty() const { return this->Size() == 0; }

    virtual String ToString() const override
    {
        std::ostringstream oss;
        oss << "(";

        bool first = true;
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (!first) { oss << ", "; } else { first = false; }
            oss << &(*it);
        }
        oss << ")";

        return String(oss.str());
    }
};

#endif // LIST_H
