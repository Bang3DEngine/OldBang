#ifndef BANG_H
#define BANG_H

//.xyz, .yxz, .zy, etc.
#define GLM_SWIZZLE
#include <glm/gtx/euler_angles.hpp>

#include "NamedEnum.h"

// MACROS

    // NONULL
        #define NONULL(x) do { if (x == nullptr) { return; } } while (0);
    //

//

#endif // BANG_H
