#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <type_traits>

#include "Bang/Bang.h"

// SFINAE ===============================================
#define T_SCALAR(T) std::is_scalar<T>::value
#define T_INTEGRAL(T) std::is_integral<T>::value
#define T_FLOATING(T) std::is_floating_point<T>::value
#define T_ENUM(T) std::is_enum<T>::value
#define T_POINTER(T) std::is_pointer<T>::value
#define T_SUBCLASS(T, BASE_CLASS) std::is_base_of<BASE_CLASS, T>::value
#define T_NOT_SUBCLASS(T, BASE_CLASS) !T_SUBCLASS(T, BASE_CLASS)

#define TT_SCALAR(T) \
    typename std::enable_if<T_SCALAR(T), T>::type
#define TT_INTEGRAL(T) \
    typename std::enable_if<T_INTEGRAL(T), T>::type
#define TT_FLOATING(T) \
    typename std::enable_if<T_FLOATING(T), T>::type
#define TT_ENUM(T) \
    typename std::enable_if<T_ENUM(T), T>::type
#define TT_POINTER(T) \
    typename std::enable_if<T_POINTER(T), T>::type
#define TT_NOT_POINTER(T) \
    typename std::enable_if<!T_POINTER(T), T>::type
#define TT_SUBCLASS(T, BASE_CLASS) \
    typename std::enable_if<T_SUBCLASS(T, BASE_CLASS), T>::type
#define TT_NOT_SUBCLASS(T, BASE_CLASS) \
    typename std::enable_if<!T_SUBCLASS(T, BASE_CLASS), T>::type

// Templated SFINAE cast dynamic/static cast
template <class SourceT, class DestT>
struct IsExplicitlyConvertible :
        public std::__and_<
                    std::is_constructible<SourceT, DestT>,
                    std::__not_<std::is_convertible<DestT, SourceT>>
                >::type
{
};

template<typename SourceT, typename DestT>
  struct IsStaticCasteable
  : public std::__or_<
                    std::is_enum<SourceT>,
                    std::is_same<SourceT, DestT>,
                    std::is_convertible<DestT, SourceT>,
                    std::is_fundamental<SourceT>,
                    std::is_fundamental< std::remove_pointer<SourceT> >,
                    std::is_fundamental< std::remove_reference<SourceT> >,
                    IsExplicitlyConvertible<DestT, SourceT>
               >::type
{
};

template<class DestT, class SourceT>
    typename std::enable_if< IsStaticCasteable<DestT, SourceT>::value, DestT >::type
Cast(SourceT x)
{ return SCAST<DestT>(x); }

template<class DestT, class SourceT>
    typename std::enable_if< !IsStaticCasteable<DestT, SourceT>::value, DestT >::type
 Cast(SourceT x)
{ return DCAST<DestT>(x); }
// =====================================================

#endif // TYPETRAITS_H
