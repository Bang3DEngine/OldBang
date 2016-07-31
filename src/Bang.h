#ifndef BANG_H
#define BANG_H


//.xyz, .yxz, .zy, etc.
#define GLM_SWIZZLE
#include <glm/gtx/euler_angles.hpp>

// MACROS

    // NONULL
        #define NONULL(x) do { if (x == nullptr) { return; } } while (0);
    //


    //BANG_ENUM
        #include "StringUtils.h"
        #include <vector>
        #include <string>
        #define BANG_ENUM(EnumName, ...) \
            enum EnumName { __VA_ARGS__ }; \
            static std::vector<std::string> EnumName##Names = StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
            static std::string EnumName##ToString(int v) { \
                return EnumName##Names[v]; \
            } \
            static std::string EnumName##ToString(EnumName v) { \
                return EnumName##ToString(int(v)); \
            } \
            static EnumName EnumName##FromString(const std::string &__name) { \
                for (int i = 0; i < EnumName##Names.size(); ++i) { \
                    if (__name == EnumName##Names[i]) { return static_cast<EnumName>(i); } \
                } \
                return static_cast<EnumName>(0); \
            }
    //
//

#endif // BANG_H
