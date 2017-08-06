#ifndef BANGFORWARD_H
#define BANGFORWARD_H

#include <type_traits>

// Asserts ==============================================
#define ENSURE(mustBeTrue) do{\
    if (!(mustBeTrue)) { return; }\
} while (false)

#define ASSERT(assertion) if ( !(assertion) ) {\
    Debug_Error("Assertion '" << #assertion << "' failed!"); exit(1); }
// =====================================================



// SFINAE ===============================================
#define T_SCALAR(T) \
    typename std::enable_if<std::is_scalar<T>::value, T>::type
#define T_FLOATING(T) \
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
#define T_ENUM(T) \
    typename std::enable_if< std::is_enum<T>::value, T>::type
#define T_SUBCLASS(T, BASE_CLASS) \
    typename std::enable_if< std::is_base_of<BASE_CLASS, T>::value, T>::type
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
FORWARD class QString;
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
using byte        = unsigned char;
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

#endif // BANGFORWARD_H
