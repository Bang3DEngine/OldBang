#ifndef BANG_H
#define BANG_H

typedef unsigned int  uint;
typedef unsigned char byte;
typedef long long EpochTime;

#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)

#endif // BANG_H
