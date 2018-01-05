#ifndef HIDEFLAGS_H
#define HIDEFLAGS_H

#include "Bang/Flags.h"

NAMESPACE_BANG_BEGIN

enum HideFlag
{
    Default,
    DontSerialize,
    DontClone
};

CREATE_FLAGS(HideFlags, HideFlag);

NAMESPACE_BANG_END

#endif // HIDEFLAGS_H
