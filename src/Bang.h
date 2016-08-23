#ifndef BANG_H
#define BANG_H

//.xyz, .yxz, .zy, etc.
#define GLM_SWIZZLE
#include <glm/gtx/euler_angles.hpp>

#include "NamedEnum.h"
#include "String.h"

// MACROS

    // NONULL
        #define NONULL(x) do { if (x == nullptr) { return; } } while (0);
    //

    // ASSERT
        #define Assert(mustBeTrue, message, returnAction) do{\
            if (!(mustBeTrue)) {\
                Debug_Error("Assertion failed: " << message);\
                returnAction;\
            }\
        } while (0)
    //

//

#endif // BANG_H
