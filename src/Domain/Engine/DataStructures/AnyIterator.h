#ifndef ANYITERATOR_H
#define ANYITERATOR_H

// Thanks to:
// https://accu.org/index.php/journals/479

#include <algorithm>

template <class T>
class AnyIterator
{
public:
    template <class IteratorClass>
    AnyIterator(const IteratorClass &rhs)
        : m_body ( new AnyIteratorSpec<IteratorClass>(rhs) )
    {
    }

    template <class IteratorClass>
    AnyIterator(IteratorClass &rhs)
        : m_body ( new AnyIteratorSpec<IteratorClass>(rhs) )
    {
    }

    AnyIterator(const AnyIterator<T> &rhs)
        : m_body ( rhs.m_body ? rhs.m_body->Clone() : nullptr )
    {
    }

    // Operators overloading
    const AnyIterator<T> operator++() // Pre-increment
    {
        AnyIterator<T> cpy = *this;
        m_body->Next();
        return cpy;
    }

    const AnyIterator<T> operator++(int) // Post-increment
    {
        m_body->Next();
        return *this;
    }

    const T& operator*() const
    {
        return m_body->Current();
    }

    AnyIterator<T>& Swap(AnyIterator<T>& rhs)
    {
        std::swap(m_body, rhs.m_body);
        return *this;
    }

    AnyIterator<T>& operator=(const AnyIterator<T>& rhs)
    {
        AnyIterator<T> cpy = AnyIterator<T>(rhs);
        Swap(cpy);
        return *this;
    }


    bool operator==(AnyIterator<T> &rhs) { return m_body->Equal(rhs.m_body); }
    bool operator==(const AnyIterator<T> &rhs) { return m_body->Equal(rhs.m_body); }
    bool operator==(AnyIterator<T> &rhs) const { return m_body->Equal(rhs.m_body); }
    bool operator==(const AnyIterator<T> &rhs) const { return m_body->Equal(rhs.m_body); }

    bool operator!=(AnyIterator<T> &rhs) { return (*this != rhs); }
    bool operator!=(const AnyIterator<T> &rhs) { return (*this != rhs); }
    bool operator!=(AnyIterator<T> &rhs) const { return (*this != rhs); }
    bool operator!=(const AnyIterator<T> &rhs) const { return (*this != rhs); }


private:
    class IAnyIterator;
    IAnyIterator *m_body;

    // IAnyIterator Interface
    class IAnyIterator
    {
    public:
        virtual IAnyIterator* Clone() const = 0;

        virtual void Next() = 0;
        virtual const T& Current() = 0;
        virtual bool Equal(const IAnyIterator *rhs) const = 0;
        virtual void Assign(const IAnyIterator *rhs) = 0;
    };


    // AnyIterator Specialized on a template class
    template<class IteratorClass>
    class AnyIteratorSpec : public IAnyIterator
    {
    public:
        AnyIteratorSpec(IteratorClass &rhs)
            : m_realIterator(rhs)
        {
        }
        AnyIteratorSpec(const IteratorClass &rhs)
            : m_realIterator(rhs)
        {
        }

        IAnyIterator* Clone () const override
        {
          return new AnyIteratorSpec<IteratorClass>(m_realIterator);
        }

        // AnyIterator Interface implementations
        virtual void Next() override { ++m_realIterator; }
        virtual const T& Current() override { return *m_realIterator; }
        virtual bool Equal(const IAnyIterator *_rhs) const override
        {
            const AnyIteratorSpec<IteratorClass>* rhs =
                    dynamic_cast<const AnyIteratorSpec<IteratorClass>*>(_rhs);
            return rhs && (m_realIterator == rhs->m_realIterator);
        }
        virtual void Assign(const IAnyIterator *rhs) override
        {
            const AnyIteratorSpec<IteratorClass>* rhsIt =
                    static_cast< const AnyIteratorSpec<IteratorClass>* >(rhs);
            m_realIterator = rhsIt->m_realIterator;
        }


    private:
        IteratorClass m_realIterator;
    };

};

#endif // ANYITERATOR_H
