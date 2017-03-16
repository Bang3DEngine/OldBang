#ifndef ICLONEABLE_H
#define ICLONEABLE_H

#define ICLONEABLE(CLASS) \
    public: \
    CLASS* CloneVirtual() const override { \
        CLASS *c = new CLASS(); \
        CloneInto(c); \
        return c; \
    } \
    CLASS* Clone() const { return static_cast<CLASS*>( CloneVirtual() ); } \
    friend class ICloneable;

class ICloneable
{
protected:
    ICloneable() {}
    virtual ICloneable* CloneVirtual() const = 0;
    virtual void CloneInto(ICloneable *clone) const = 0;
};

#endif // ICLONEABLE_H
