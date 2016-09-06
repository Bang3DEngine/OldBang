#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <algorithm>

class String;
template <class T> class List;
template <class T>
class Array : private std::vector<T>
{
public:
    typedef typename std::vector<T>::iterator Iterator;
    typedef typename std::vector<T>::const_iterator Const_Iterator;
    typedef typename std::vector<T>::reverse_iterator Reverse_Iterator;
    typedef typename std::vector<T>::const_reverse_iterator Const_Reverse_Iterator;

    Array()
    {
    }

    Array(const std::vector<T> &v) : std::vector<T>(v)
    {
    }

    Array(int size) : std::vector<T>(size)
    {
    }

    // Allow "v = {1,2,3...}" initialization
    Array(std::initializer_list<T> l) : std::vector<T>(l)
    {
    }

    void Clear()
    {
        this->clear();
    }
    bool Empty() const
    {
        return this->Size() == 0;
    }
    unsigned int Size() const
    {
        return this->size();
    }
    void Resize(int n)
    {
        this->resize(n);
    }

    Iterator Remove(Iterator it)
    {
        return this->erase(it);
    }
    Iterator Remove(Iterator first, Iterator last)
    {
        return this->erase(first, last);
    }


    void Add(const T& x)
    {
        this->PushBack(x);
    }
    void PushBack(const T& x)
    {
        this->push_back(x);
    }
    T& PopBack()
    {
        T& x = Back();
        this->pop_back();
        return x;
    }

    const T& Front() const
    {
        return this->front();
    }
    const T& Back() const
    {
        return this->back();
    }
    T& Front()
    {
        return this->front();
    }
    T& Back()
    {
        return this->back();
    }

    List<T> ToList() const
    {
        List<T> list;
        for (const T &x : *this)
        {
            list.PushBack(x);
        }
        return list;
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
    Iterator begin() { return this->std::vector<T>::begin(); }
    Iterator end() { return this->std::vector<T>::end(); }
    Const_Iterator begin() const { return this->std::vector<T>::begin(); }
    Const_Iterator end() const { return this->std::vector<T>::end(); }

    bool Contains(const T& x) const
    {
        return std::find(Begin(), End(), x) != End();
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

    const T* Data() const
    {
        return this->data();
    }

    T& operator[](int i)
    {
        return std::vector<T>::operator [](i);
    }
    const T& operator[](int i) const
    {
        return std::vector<T>::operator [](i);
    }

    String ToString();
};

#endif // ARRAY_H
