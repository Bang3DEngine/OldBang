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


#define T_SCALAR(T) typename std::enable_if<std::is_scalar<T>::value, T>::type
#define T_FLOATING(T) typename std::enable_if<std::is_floating_point<T>::value, T>::type

#define DCAST dynamic_cast
#define SCAST static_cast

#endif // BANG_H
