#ifndef CONTAINER_H
#define CONTAINER_H

#include <algorithm>
#include <functional>

#include "Bang/Bang.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/Collection.h"

#define CONTAINER(STLContainerClass) \
    private: typedef class Container< STLContainerClass, T > Container; \
    public: \
    typedef typename Container::Iterator               Iterator; \
    typedef typename Container::Const_Iterator         Const_Iterator; \
    typedef typename Container::Reverse_Iterator       Reverse_Iterator; \
    typedef typename Container::Const_Reverse_Iterator Const_Reverse_Iterator;\
    private:

template <
  template <class T,
            class Alloc = std::allocator<T> > class STLContainerClass,
  class T,
  class Alloc = std::allocator<T> >
class Container : public IToString,
                  public Collection<T>,
                  public STLContainerClass<T>
{
public:
    virtual ~Container() {}

    typedef typename STLContainerClass<T>::iterator               Iterator;
    typedef typename STLContainerClass<T>::const_iterator         Const_Iterator;
    typedef typename STLContainerClass<T>::reverse_iterator       Reverse_Iterator;
    typedef typename STLContainerClass<T>::const_reverse_iterator Const_Reverse_Iterator;

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end();   }
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


    void Add(const T& x) { this->PushBack(x); }
    void PushBack(const T& x)  { this->push_back(x);  }
    template< template <class OtherT = T, class OtherAlloc = Alloc>
                        class OtherSTLContainerClass = STLContainerClass,
              class OtherT = T>
    void Add(const Container<OtherSTLContainerClass, OtherT>& collection)
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

    template <class TIterator = Iterator>
    Iterator FindLast(const T& x)
    {
        for (auto it = this->RBegin(); it != this->REnd(); ++it)
        {
            if (*it == x)
            {
                AnyIterator<T> res = it.base();
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

    Iterator Remove(const T &x) { return this->Remove( this->Find(x) ); }

    template<class TIterator = Iterator>
    Iterator Remove(TIterator it) { return this->erase(it); }

    template<class TIterator = Iterator>
    Iterator Remove(const TIterator &first, const TIterator &last)
    {
        return this->erase(first, last);
    }

    template<class TIterator = Iterator>
    Iterator Remove(const T& x)
    {
        TIterator it = this->Find(x);
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
    Container() {}

    explicit Container(std::initializer_list<T> l) : STLContainerClass<T>(l)
    {
    }

    explicit Container(const STLContainerClass<T> &v) : STLContainerClass<T>(v)
    {
    }

    explicit Container(int size) : STLContainerClass<T>(size)
    {
    }

    explicit Container(int size, const T& initValue)
        : STLContainerClass<T>(size, initValue)
    {
    }

//    explicit Container(OtherIterator begin, OtherIterator end)
//        : STLContainerClass<T>(begin, end)
//    {
//    }

//    explicit Container(const Container &collection)
//        : Container(collection.Begin(), collection.End())
//    {
//    }

    virtual char GetToStringOpeningBrace() const { return '('; }
    virtual char GetToStringClosingBrace() const { return ')'; }

private:
    // We need these 2 overloaded functions to let ToString work with classes
    // that do not implement IToString
    static String PassToString(const IToString &x) { return x.ToString(); }
    static String PassToString(const T &x) { return String::ToString(&x); }
};

#endif // CONTAINER_H
