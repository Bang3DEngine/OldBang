#ifndef BANG_H
#define BANG_H

typedef unsigned int  uint;
typedef unsigned char byte;
typedef long long EpochTime;

#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)

#define BP_REFLECT_VARIABLE(args...)
#define BP_REFLECT_CLASS(args...)
#define BP_REFLECT_STRUCT(args...)
#define BPRED(boolPredicate) ( [&](const auto &x) { return (boolPredicate); } )

#endif // BANG_H
