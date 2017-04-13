#ifndef ICLONEABLE_H
#define ICLONEABLE_H

#define ICLONEABLE(CLASS) \
    public: \
    virtual CLASS* Clone() const override { \
        CLASS *c = new CLASS(); \
        CloneInto(c); \
        return c; \
    }

class ICloneable
{
public:
    virtual ICloneable* Clone() const = 0;

protected:
    ICloneable() {}
    virtual void CloneInto(ICloneable *clone) const = 0;
};

#endif // ICLONEABLE_H
