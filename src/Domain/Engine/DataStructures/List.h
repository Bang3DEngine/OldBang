#ifndef LIST_H
#define LIST_H

#include <set>
#include <list>
#include <sstream>
#include <algorithm>

class String;
template <class T> class Array;
template <class T>
class List : private std::list<T>
{
public:
    typedef typename std::list<T>::iterator Iterator;
    typedef typename std::list<T>::const_iterator Const_Iterator;
    typedef typename std::list<T>::reverse_iterator Reverse_Iterator;
    typedef typename std::list<T>::const_reverse_iterator Const_Reverse_Iterator;

    List()
    {
    }

    List(const std::list<T> &l) : std::list<T>(l)
    {
    }

    // Allow "l = {1,2,3...}" initialization
    List(std::initializer_list<T> l) : std::list<T>(l)
    {
    }

    bool Contains(const T& x) const
    {
        return std::find(Begin(), End(), x) != End();
    }

    void Splice(Iterator insertAfter, List &from)
    {
        this->splice(insertAfter, from);
    }
    void Splice(Iterator insertAfter, List &from, Iterator element)
    {
        this->splice(insertAfter, from, element);
    }
    void Splice(Iterator insertAfter, List &src,
                Iterator srcRangeBegin, Iterator srcRangeEnd)
    {
        this->splice(insertAfter, src, srcRangeBegin, srcRangeEnd);
    }


    void Clear()
    {
        this->clear();
    }

    int Size() const
    {
        return this->size();
    }
    bool Empty() const
    {
        return this->Size() == 0;
    }

    const T& Front() const
    {
        return this->front();
    }
    const T& Back() const
    {
        return this->back();
    }

    void Add(const T& x)
    {
        this->PushBack(x);
    }

    void Add(const List<T>& xList)
    {
        for (const T &x : xList)
        {
            this->Add(x);
        }
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
        return End();
    }

    int CountOccurrences(const T& x)
    {
        int c = 0;
        for (auto it = Begin(); it != End(); ++it)
        {
            if (x == *it) { ++c; }
        }
        return c;
    }

    void Insert(Iterator it, const T& x)
    {
        this->InsertBefore(it, x);
    }
    void Insert(Iterator first, int count, const T& x)
    {
        this->insert(first, count, x);
    }
    void InsertBefore(Iterator it, const T& x)
    {
        this->insert(it, x);
    }
    void InsertAfter(Iterator it, const T& x)
    {
        Iterator itAfter = it;
        std::advance(itAfter, 1);
        this->insert(itAfter, x);
    }

    Iterator Remove(const T& x)
    {
        Iterator it = Find(x);
        if (it != End())
        {
            return this->erase(it);
        }
        return this->End();
    }

    void RemoveAll(const T& x)
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            if (*it == x)
            {
                it = this->erase(it);
                --it;
            }
        }
    }

    Iterator Remove(Iterator it)
    {
        return this->erase(it);
    }
    Iterator Remove(Iterator first, Iterator last)
    {
        return this->erase(first, last);
    }

    const T& PopBack()
    {
        const T& back = Back();
        this->pop_back();
        return back;
    }
    const T& PopFront()
    {
        const T& front = Front();
        this->pop_front();
        return front;
    }
    void PushFront(const T& x)
    {
        this->push_front(x);
    }
    void PushBack(const T& x)
    {
        this->push_back(x);
    }
    void PushBack(const List<T>& l)
    {
        this->Add(l);
    }

    List<T> Concat(const List<T> &l) const
    {
        List<T> result = *this;
        for(const T& x : l) { result.PushBack(x); }
        return result;
    }

    Array<T> ToArray() const
    {
        Array<T> array;
        for (const T &x : *this)
        {
            array.PushBack(x);
        }
        return array;
    }

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end(); }
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

    String ToString();
};

#endif // LIST_H
