#ifndef ICLONEABLE_H
#define ICLONEABLE_H

#define ICLONEABLE(CLASS) \
    CLASS* Clone() const { \
        T *c = new T(); \
        CloneInto(c); \
        return c; \
    } \
    friend class ICloneable;

class ICloneable
{
public:
    template <class T>
    T* Clone() const
    {
        return static_cast<T*>( CloneVirtual() );
    }


protected:
    ICloneable() {}

    virtual ICloneable* CloneVirtual() const = 0;
    virtual void CloneInto(ICloneable *clone) const = 0;

    template <class T>
    T* _Clone() const
    {
        T *c = new T();
        CloneInto(c);
        return c;
    }
};

#endif // ICLONEABLE_H
