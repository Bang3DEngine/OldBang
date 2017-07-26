#ifndef BANG_H
#define BANG_H

#include <type_traits>

typedef unsigned int  uint;
typedef unsigned char byte;
typedef long long EpochTime;
typedef const char* QSlot;

#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)


// #define SCALAR_TYPE(TYPE) typename std::enable_if<std::is_arithmetic<TYPE>::value, TYPE>::type
#define SCALAR_TYPE(TYPE) typename std::enable_if<true, TYPE>::type
#define SCALAR_TYPE_H(TYPE) typename std::enable_if<true, TYPE>::type

#define DCAST dynamic_cast
#define SCAST static_cast

#endif // BANG_H
