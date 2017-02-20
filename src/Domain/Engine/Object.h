#ifndef OBJECT_H
#define OBJECT_H

#include "HideFlags.h"

class Object
{
public:
    Object();
    virtual ~Object();

    const HideFlags &GetHideFlags() const;
    bool HasHideFlag(const HideFlags &hideFlag) const;

    void AddHideFlag(const HideFlags &hideFlag);
    void RemoveHideFlag(const HideFlags &hideFlag);

private:
    HideFlags m_hideFlags = HideFlags::None;
};

#endif // OBJECT_H
