#ifndef BANG_H
#define BANG_H


#define NONULL(x) do { if(x == nullptr) { return; } } while(0);

//.xyz, .yxz, .zy, etc.
#define GLM_SWIZZLE
#include <glm/gtx/euler_angles.hpp>

#define BANG_EDITOR

#endif // BANG_H
