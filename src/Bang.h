#ifndef BANG_H
#define BANG_H


//.xyz, .yxz, .zy, etc.
#define GLM_SWIZZLE
#include <glm/gtx/euler_angles.hpp>

// MACROS

    // NONULL
        #define NONULL(x) do { if (x == nullptr) { return; } } while (0);
    //


    //NAMED_ENUM
        #include <map>
        #include <vector>
        #include <string>

        #include "StringUtils.h"

        #define NAMED_ENUM(EnumName, ...) \
            \
            enum EnumName { __VA_ARGS__ }; \
            \
            static inline int _GetEnumName##Size() { \
                static const std::vector<std::string> names = StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
                return names.size(); \
            } \
            \
            static std::string _GetEnumName##NameByIndex(int index) { \
                static const std::vector<std::string> names = \
                            StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
                return names[index]; \
            } \
            static std::string _GetEnumName##NameByValue(int enumValue) { \
                static const std::vector<std::string> names = \
                            StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
                static const std::vector<int> values = /* Holds the user values of each Enum entry */ \
                            StringUtils::BangEnumVariadicStringToValuesArray(#__VA_ARGS__); \
                static std::map<int,int> enumValueToIndex; /* For every enum value, it maps to the index inside the Enum. */ \
                static bool mapInit = false; \
                if (!mapInit) \
                { \
                    for (int i = 0; i < names.size(); ++i) \
                    { \
                        enumValueToIndex[values[i]] = i;  \
                    } \
                    mapInit = true; \
                } \
                return names[enumValueToIndex[enumValue]]; \
            } \
            \
            static std::string EnumName##ToString(int v) { \
                return _GetEnumName##NameByValue(v); \
            } \
            \
            static std::string EnumName##ToString(EnumName v) { \
                return EnumName##ToString(int(v)); \
            } \
            \
            static EnumName EnumName##FromString(const std::string &__name) { \
                for (int i = 0; i < _GetEnumName##Size(); ++i) { \
                    if (__name == _GetEnumName##NameByIndex(i)) { return static_cast<EnumName>(i); } \
                } \
                return static_cast<EnumName>(0); \
            }
    //
//

#endif // BANG_H
