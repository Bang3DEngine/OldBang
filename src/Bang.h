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

        #define NamedEnum(EnumName, ...) \
            \
            enum EnumName { __VA_ARGS__ }; \
            \
            static const std::string EnumName##_GetEnumName() { \
                return #EnumName; \
            } \
            static const std::vector<std::string>& EnumName##_GetNamesVector() { \
                static const std::vector<std::string> names = StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
                return names; \
            } \
            \
            static const std::vector<int>& EnumName##_GetValuesVector() { \
                static const std::vector<int> values = /* Holds the user values of each Enum entry */ \
                            StringUtils::BangEnumVariadicStringToValuesArray(#__VA_ARGS__); \
                return values; \
            } \
            \
            static std::map<int,int>& EnumName##_GetEnumValueToIndexMap() { /* For every enum value, it maps to the index inside the Enum. */ \
                static bool mapInit = false; \
                static std::map<int,int> enumValueToIndex; \
                if (!mapInit) \
                { \
                    const std::vector<std::string>& names = EnumName##_GetNamesVector(); \
                    const std::vector<int>& values = EnumName##_GetValuesVector(); \
                    for (int i = 0; i < names.size(); ++i) \
                    { \
                        enumValueToIndex[values[i]] = i;  \
                    } \
                    mapInit = true; \
                } \
                return enumValueToIndex; \
            } \
            static inline int EnumName##_GetSize() { \
                return EnumName##_GetNamesVector().size(); \
            } \
            \
            static std::string EnumName##_GetValueByIndex(int index) { \
                return EnumName##_GetNamesVector()[index]; \
            } \
            \
            static std::string EnumName##_GetNameByIndex(int index) { \
                return EnumName##_GetNamesVector()[index]; \
            } \
            \
            static int EnumName##_GetIndexFromValue(int value) { \
                std::map<int,int>& enumValueToIndex = EnumName##_GetEnumValueToIndexMap(); \
                return enumValueToIndex[value]; \
            } \
            static std::string EnumName##_GetNameByValue(int enumValue) { \
                const std::vector<std::string>& names = EnumName##_GetNamesVector(); \
                return names[EnumName##_GetIndexFromValue(enumValue)]; \
            } \
            \
            static std::string EnumName##_ToString(int v) { \
                return EnumName##_GetNameByValue(v); \
            } \
            \
            static std::string EnumName##_ToString(EnumName v) { \
                return EnumName##_ToString(int(v)); \
            } \
            \
            static EnumName EnumName##_FromString(const std::string &name) { \
                const std::vector<int>& values = EnumName##_GetValuesVector(); \
                for (int i = 0; i < EnumName##_GetSize(); ++i) { \
                    if (name == EnumName##_GetNameByIndex(i)) { return static_cast<EnumName>(values[i]); } \
                } \
                return static_cast<EnumName>(0); \
            }
    //
//

#endif // BANG_H
