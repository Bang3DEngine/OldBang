#ifndef BANGFORWARD_H
#define BANGFORWARD_H

#include <iostream>
#include <type_traits>

// Asserts ==============================================
#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)

#define ASSERT(assertion) if ( !(assertion) ) {\
  std::cerr << "ASSERTION FAILED: '" << #assertion << "' failed!"; abort(); \
}
// =====================================================



// SFINAE ===============================================
#define T_SCALAR(T) std::is_scalar<T>::value
#define T_INTEGRAL(T) std::is_integral<T>::value
#define T_FLOATING(T) std::is_floating_point<T>::value
#define T_ENUM(T) std::is_enum<T>::value
#define T_SUBCLASS(T, BASE_CLASS) std::is_base_of<BASE_CLASS, T>::value

#define TT_SCALAR(T) \
    typename std::enable_if<T_SCALAR(T), T>::type
#define TT_INTEGRAL(T) \
    typename std::enable_if<T_INTEGRAL(T), T>::type
#define TT_FLOATING(T) \
    typename std::enable_if<T_FLOATING(T), T>::type
#define TT_ENUM(T) \
    typename std::enable_if<T_ENUM(T), T>::type
#define TT_SUBCLASS(T, BASE_CLASS) \
    typename std::enable_if<T_SUBCLASS(T, BASE_CLASS), T>::type
// =====================================================



// Casts ===============================================
#define SCAST static_cast
#define DCAST dynamic_cast
#define RCAST reinterpret_cast
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
FORWARD_T class RectG;
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

using Rectf       = RectG<float>;
using Rectd       = RectG<double>;
using Recti       = RectG<int>;
using Rect        = Rectf;
// ======================================================

// Others =================================

#define PROPAGATE(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
        (*it)->FUNCTION; \
} while (0)

// ======================================================

#endif // BANGFORWARD_H
