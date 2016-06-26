#ifndef ICLONEABLE_H
#define ICLONEABLE_H

class ICloneable
{
protected:
    ICloneable() {}
    virtual void CloneInto(ICloneable *clone) const = 0;

public:
    virtual ICloneable* Clone() const = 0;
};

#endif // ICLONEABLE_H
