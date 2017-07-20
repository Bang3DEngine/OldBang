#ifndef BANG_H
#define BANG_H

typedef unsigned int  uint;
typedef unsigned char byte;
typedef long long EpochTime;
typedef const char* QSlot;

#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)

#define DCAST dynamic_cast
#define SCAST static_cast

#endif // BANG_H
