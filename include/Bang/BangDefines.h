#ifndef BANGFORWARD_H
#define BANGFORWARD_H

#include <cstdint>
#include <iostream>

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

#define NAMESPACE_NAME       Bang
#define USING_NAMESPACE_BANG using namespace NAMESPACE_NAME;
#define NAMESPACE_BANG_BEGIN namespace NAMESPACE_NAME {
#define NAMESPACE_BANG_END   }

NAMESPACE_BANG_BEGIN

#ifndef NDEBUG
#define DEBUG
#endif

// Asserts ==============================================
#ifdef DEBUG
#define ASSERT_MSG(assertion, msg) if ( !(assertion) ) {\
  std::cerr << "BANG ASSERTION FAILED: '" << msg; abort(); \
}
#else
#define ASSERT_MSG(assertion, msg) // No Assert in release
#endif

#define ASSERT_SOFT(assertion) if ( !(assertion) ) {\
    std::cerr << "BANG SOFT ASSERTION FAILED.'" << std::endl; \
}
#define ASSERT_SOFT_MSG(assertion, msg) if ( !(assertion) ) {\
    std::cerr << "BANG SOFT ASSERTION FAILED: '" << msg << std::endl; \
}

#define ASSERT(assertion) ASSERT_MSG(assertion, #assertion)

#define SASSERT(assertion) static_assert(assertion)
// =====================================================

// Casts ===============================================
#define SCAST static_cast
#define DCAST dynamic_cast
#define RCAST reinterpret_cast
#define GLCAST(x) static_cast<GLenum>(x)
// =====================================================


// Frequent class forwardings ==========================
#define FORWARD
#define FORWARD_T  template<class>
#define FORWARD_TT template<class,class>

FORWARD class Mesh;
FORWARD class Path;
FORWARD class Color;
FORWARD class AABox;
FORWARD class String;
FORWARD class Sphere;
FORWARD class XMLNode;
FORWARD class Material;
FORWARD class Transform;
FORWARD class GameObject;
FORWARD class ICloneable;

FORWARD_TT class Map;
FORWARD_T  class Set;
FORWARD_T  class List;
FORWARD_T  class Array;

FORWARD   class File;
FORWARD_T class AARectG;
FORWARD_T class Vector2G;
FORWARD_T class Vector3G;
FORWARD_T class Vector4G;
FORWARD_T class Matrix3G;
FORWARD_T class Matrix4G;
FORWARD_T class QuaternionG;
//====================================================


// Types ================================================
using uint        = unsigned int;
using Byte        = unsigned char;
using GLId        = uint;
using BoolByte    = Byte;
using EpochTime   = long long;

using Vector2i    = Vector2G<int>;
using Vector2f    = Vector2G<float>;
using Vector2d    = Vector2G<double>;
using Vector2     = Vector2f;

using Vector3i    = Vector3G<int>;
using Vector3f    = Vector3G<float>;
using Vector3d    = Vector3G<double>;
using Vector3     = Vector3f;

using Vector4i    = Vector4G<int>;
using Vector4f    = Vector4G<float>;
using Vector4d    = Vector4G<double>;
using Vector4     = Vector4f;

using Quaternionf = QuaternionG<float>;
using Quaterniond = QuaternionG<double>;
using Quaternion  = Quaternionf;

using Matrix4f    = Matrix4G<float>;
using Matrix4d    = Matrix4G<double>;
using Matrix4     = Matrix4f;

using Matrix3f    = Matrix3G<float>;
using Matrix3d    = Matrix3G<double>;
using Matrix3     = Matrix3f;

using AARectf     = AARectG<float>;
using AARectd     = AARectG<double>;
using AARecti     = AARectG<int>;
using AARect      = AARectf;
// ======================================================

// Other ================================================
template<class EnumT> constexpr EnumT Undef() { return SCAST<EnumT>(-1); }
// ======================================================

// Macro overloading ====================================
#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,  \
                _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
                _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
                _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
                _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
                _61,_62,_63,N,...) N
#define __RSEQ_N() 63,62,61,60, 59,58,57,56,55,54,53,52,51,50, \
                   49,48,47,46,45,44,43,42,41,40, \
                   39,38,37,36,35,34,33,32,31,30, \
                   29,28,27,26,25,24,23,22,21,20, \
                   19,18,17,16,15,14,13,12,11,10, \
                   9,8,7,6,5,4,3,2,1,0

// General definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

// Example definition of overloaded macro
/*
#define ___ADD(...) VFUNC(___ADD, __VA_ARGS__)
#define ___ADD1(a)     (a)
#define ___ADD2(a,b)   (a+b)
#define ___ADD3(a,b,c) (a+b+c)
*/
// ======================================================

NAMESPACE_BANG_END

#endif // BANGFORWARD_H
