#ifndef LIST_H
#define LIST_H

#include <list>

#include "Bang/Container.h"

template <class T>
class List : public Container< std::list, T >
{
    CONTAINER(std::list)
public:
    List() { }
    List(const std::list<T> &l)         : Container(l) { }
    List(int size)                      : Container(size) { }
    List(int size, const T& initValue)  : Container(size, initValue) { }
    List(std::initializer_list<T> l)    : Container(l) { }
    virtual ~List() {}

    /*
    operator Collection(const Collection<T> &col) const
    {
        List<T> l;
        l.Add(col);
        return l;
    }
    */

    template <class Iterator>
    void Splice(Iterator insertAfter, List<T> &from)
    {
        this->splice(insertAfter, from);
    }

    template <class Iterator>
    void Splice(Iterator insertAfter, List<T> &from, Iterator element)
    {
        this->splice(insertAfter, from, element);
    }

    template <class Iterator>
    void Splice(Iterator insertAfter, List<T> &src,
                Iterator srcRangeBegin, Iterator srcRangeEnd)
    {
        this->splice(insertAfter, src, srcRangeBegin, srcRangeEnd);
    }

    template<class Iterator>
    void Insert(const Iterator& it, const T& x)
    {
        this->InsertBefore(it, x);
    }

    template<class Iterator>
    void Insert(const Iterator& first, int count, const T& x)
    {
        this->insert(first, count, x);
    }

    template<class Iterator>
    void InsertBefore(const Iterator& it, const T& x)
    {
        this->insert(it, x);
    }

    template<class Iterator>
    void InsertAfter(const Iterator& it, const T& x)
    {
        AnyIterator<T> itAfter = it;
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

public:
    char GetToStringOpeningBrace() const override { return '('; }
    char GetToStringClosingBrace() const override { return ')'; }
};

#endif // LIST_H
