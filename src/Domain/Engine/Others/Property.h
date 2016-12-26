#ifndef PROPERTY_H
#define PROPERTY_H

/*
template<class PROPERTY_CLASS, class HOLDER_CLASS>
class Property
{
    //typedef std::function<PROPERTY_CLASS()> Getter;
    typedef std::function<PROPERTY_CLASS(const HOLDER_CLASS*)> Getter;
    //typedef PROPERTY_CLASS (HOLDER_CLASS::* Getter)() const ;
    //typedef std::function<void(PROPERTY_CLASS)> Setter;
    typedef std::function<void(const HOLDER_CLASS*, PROPERTY_CLASS)> Setter;
    //typedef void (HOLDER_CLASS::* Setter)(PROPERTY_CLASS) const ;

    HOLDER_CLASS *m_objInstance;
    std::function<PROPERTY_CLASS(HOLDER_CLASS*)> m_getter;
    std::function<void(HOLDER_CLASS*, PROPERTY_CLASS)> m_setter;

public:
    Property(HOLDER_CLASS *objInstance, Getter getter, Setter setter)
             :  m_objInstance(objInstance), m_getter(getter), m_setter(setter)
    {
    }

    operator PROPERTY_CLASS()
    {
        return m_getter(*m_objInstance);
    }

    void operator =(PROPERTY_CLASS value)
    {
        m_setter(*m_objInstance, value);
    }
};
*/

template<typename T>
class Property
{
private:
    T& _value;

public:
    Property(T& value) : _value(value)
    {
    }

    Property<T>& operator = (const T& val)
    {
        _value = val;
        return *this;
    }

    operator const T&() const
    {
        return _value;
    }
};
#endif // PROPERTY_H
