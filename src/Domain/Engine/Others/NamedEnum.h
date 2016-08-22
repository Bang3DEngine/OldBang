#ifndef NAMEDENUM_H
#define NAMEDENUM_H

#include <map>
#include <vector>


#include "StringUtils.h"

#define NamedEnum(EnumName, ...) \
\
enum class EnumName { __VA_ARGS__ }; \
\
/* Returns the name of the enum itself. */ \
static const String EnumName##_GetEnumName() { \
    return #EnumName; \
} \
\
/* Returns the array of names of the enum. */ \
static const std::vector<String>& EnumName##_GetNamesVector() { \
    static const std::vector<String> names = StringUtils::BangEnumVariadicStringToNamesArray(#__VA_ARGS__); \
    return names; \
} \
\
/* Returns the array of values of the enum. */ \
static const std::vector<EnumName>& EnumName##_GetValuesVector() { \
    static const std::vector<EnumName> values = /* Holds the user values of each Enum entry */ \
                StringUtils::BangEnumVariadicStringToValuesArray<EnumName>(#__VA_ARGS__); \
    return values; \
} \
\
/* For every enum value, it maps to the index inside the Enum. */ \
static std::map<EnumName,int>& EnumName##_GetEnumValueToIndexMap() { \
    static bool mapInit = false; \
    static std::map<EnumName,int> enumValueToIndex; \
    if (!mapInit) \
    { \
        const std::vector<String>& names = EnumName##_GetNamesVector(); \
        const std::vector<EnumName>& values = EnumName##_GetValuesVector(); \
        for (int i = 0; i < names.size(); ++i) \
        { \
            enumValueToIndex[values[i]] = i;  \
        } \
        mapInit = true; \
    } \
    return enumValueToIndex; \
} \
\
/* Returns the number of fields of the enum. */ \
static inline int EnumName##_GetSize() { \
    return EnumName##_GetNamesVector().size(); \
} \
\
/* Given an index return its value in the enum. */ \
static String EnumName##_GetValueByIndex(int index) { \
    return EnumName##_GetNamesVector()[index]; \
} \
\
/* Given an index return its name in the enum. */ \
static String EnumName##_GetNameByIndex(int index) { \
    return EnumName##_GetNamesVector()[index]; \
} \
\
/* Given a value, return its index in the enum. */ \
static int EnumName##_GetIndexFromValue(EnumName value) { \
    std::map<EnumName,int>& enumValueToIndex = EnumName##_GetEnumValueToIndexMap(); \
    return enumValueToIndex[value]; \
} \
\
/* Given a value, return its name in the enum. */ \
static String EnumName##_GetNameByValue(EnumName enumValue) { \
    const std::vector<String>& names = EnumName##_GetNamesVector(); \
    return names[EnumName##_GetIndexFromValue(enumValue)]; \
} \
\
/* Given a value as an int, return its name in the enum. */ \
static String EnumName##_ToString(int v) { \
    return  EnumName##_ToString(static_cast<EnumName>(v)); \
} \
\
/* Given a value, return its name in the enum. */ \
static String EnumName##_ToString(EnumName v) { \
    return EnumName##_GetNameByValue(v); \
} \
\
/* Given a string, return its value in the enum. */ \
static EnumName EnumName##_FromString(const String &name) { \
    const std::vector<EnumName>& values = EnumName##_GetValuesVector(); \
    for (int i = 0; i < EnumName##_GetSize(); ++i) { \
        if (name == EnumName##_GetNameByIndex(i)) { return static_cast<EnumName>(values[i]); } \
    } \
    return static_cast<EnumName>(0); \
}

#endif // NAMEDENUM_H
