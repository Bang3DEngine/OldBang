#ifndef HIDEFLAGS_H
#define HIDEFLAGS_H

#include "Bang/Flags.h"

enum HideFlag
{
    Default,
    DontSave
};

CREATE_FLAGS(HideFlags, HideFlag);

#endif // HIDEFLAGS_H
