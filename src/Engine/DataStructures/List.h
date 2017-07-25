#ifndef LIST_H
#define LIST_H

#include <list>
#include <functional>

#include "Bang/Collection.h"

template <class T>
class List : public std::list<T>
{
public:
    typedef typename std::list<T>::iterator               Iterator;
    typedef typename std::list<T>::const_iterator         Const_Iterator;
    typedef typename std::list<T>::reverse_iterator       Reverse_Iterator;
    typedef typename std::list<T>::const_reverse_iterator Const_Reverse_Iterator;

    List() { }
    List(const std::list<T> &l)         : std::list<T>(l) { }
    List(int size)                      : std::list<T>(size) { }
    List(int size, const T& initValue)  : std::list<T>(size, initValue) { }
    List(std::initializer_list<T> l)    : std::list<T>(l) { }

    template <class OtherIterator>
    List(OtherIterator begin, OtherIterator end)
        : std::list<T>(begin, end()) {}

    template <class OtherIterator>
    List(const CollectionRange<OtherIterator> &col)
        : List(col.Begin(), col.End()) {}

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

    Iterator Remove(const Iterator &first, const Iterator &last)
    {
        return this->erase(first, last);
    }
    Iterator Remove(Iterator it) { return this->erase(it); }
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

    void Resize(int n) { this->resize(n); }
    uint Size() const  { return this->size(); }
    void Clear()       { this->clear(); }
    bool IsEmpty() const { return this->Size() == 0; }

    CollectionRange<Iterator> GetRangeAll()
    {
        return CollectionRange<Iterator>(this->Begin(), this->End());
    }
    CollectionRange<Const_Iterator> GetRangeAll() const
    {
        return CollectionRange<Const_Iterator>(this->Begin(), this->End());
    }

    operator CollectionRange<Iterator>() { return GetRangeAll(); }
    operator CollectionRange<Iterator>() const { return GetRangeAll(); }
};

#endif // LIST_H
