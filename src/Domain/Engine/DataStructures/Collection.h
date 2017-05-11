#ifndef COLLECTION_H
#define COLLECTION_H

#include <vector>
#include <algorithm>

#include "Bang/Bang.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

class ICollection
{
protected:
    ICollection();
    virtual ~ICollection();
};


template <
  template <class T,
            class Alloc = std::allocator<T> > class STLContainerClass,
  class T,
  class Alloc = std::allocator<T>
>
class Collection : public ICollection,
                   public IToString,
                   public STLContainerClass< T >
{
public:
    typedef typename
          STLContainerClass<T>::iterator               Iterator;
    typedef typename
          STLContainerClass<T>::const_iterator         Const_Iterator;
    typedef typename
          STLContainerClass<T>::reverse_iterator       Reverse_Iterator;
    typedef typename
          STLContainerClass<T>::const_reverse_iterator Const_Reverse_Iterator;

    Collection(std::initializer_list<T> l) : STLContainerClass<T>(l) { }
    Collection(const std::vector<T> &v) : STLContainerClass<T>(v) { }
    Collection(int size) : STLContainerClass<T>(size) { }
    Collection(int size, const T& initValue)
        : STLContainerClass<T>(size, initValue) { }

    virtual void Add(const T &x) {}

    Iterator Begin() { return this->begin(); }
    Iterator End()   { return this->end();   }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    Reverse_Iterator RBegin() { return this->rbegin(); }
    Reverse_Iterator REnd() { return this->rend(); }
    Const_Reverse_Iterator RBegin() const { return this->rbegin(); }
    Const_Reverse_Iterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->STLContainerClass<T>::begin(); }
    Iterator end() { return this->STLContainerClass<T>::end(); }
    Const_Iterator begin() const { return this->STLContainerClass<T>::begin(); }
    Const_Iterator end() const { return this->STLContainerClass<T>::end(); }

    T& operator[](int i)
    {
        return STLContainerClass<T>::operator [](i);
    }
    const T& operator[](int i) const
    {
        return STLContainerClass<T>::operator [](i);
    }

    const T* Data() const { return this->data(); }

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

    bool Contains(const T& x) const
    {
        return std::find(this->Begin(), this->End(), x) != this->End();
    }

    template < template <class> class OtherContainerClass, class OtherT = T >
    OtherContainerClass<OtherT> To() const
    {
        OtherContainerClass<OtherT> cont;
        for (const T &x : *this) { cont.Add( OtherT(x) ); }
        return cont;
    }

    Iterator Remove(Iterator it) { return this->erase(it); }
    Iterator Remove(Iterator first, Iterator last)
    {
        return this->erase(first, last);
    }

    void Resize(int n) { this->resize(n); }
    uint Size() const  { return this->size(); }
    void Clear()       { this->clear(); }
    bool Empty() const { return this->Size() == 0; }

    virtual String ToString() const override
    {
        std::ostringstream oss;
        oss << GetToStringOpeningBrace();

        bool first = true;
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (!first) { oss << ", "; } else { first = false; }
            oss << PassToString(*it);
        }
        oss << GetToStringClosingBrace();

        return String(oss.str());
    }

protected:
    Collection() {}
    virtual ~Collection() {}

    virtual char GetToStringOpeningBrace() const { return '('; }
    virtual char GetToStringClosingBrace() const { return ')'; }

private:
    // We need these 2 overloaded functions to let ToString work with classes
    // that do not implement IToString
    static String PassToString(const IToString &x) { return x.ToString(); }
    static String PassToString(const T &x) { return String::ToString(&x); }
};

#endif // COLLECTION_H
