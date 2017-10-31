#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <limits>

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Math
{
public:
    static constexpr double PI = 3.1415926535897932384626;

    template<class T>
    static T Min()
    {
        return std::numeric_limits<T>::min();
    }

    template<class T>
    static T Max()
    {
        return std::numeric_limits<T>::max();
    }

    template<class T>
    static T Infinity()
    {
        return std::numeric_limits<T>::infinity();
    }

    template<class T>
    static T NegativeInfinity()
    {
        return -std::numeric_limits<T>::infinity();
    }

    template<class T>
    static T IsInfinity(T value)
    {
        return std::isinf(value);
    }

    template<class T>
    static T IsNaN(T value)
    {
        return std::isnan(value);
    }

    template<class T>
    static T Min(const T &a, const T &b)
    {
        return (a <= b) ? a : b;
    }

    template<class T>
    static T Max(const T &a, const T &b)
    {
        return (a >= b) ? a : b;
    }

    template<class T, class T2, class T3>
    static T Clamp(const T &value, const T2 &min, const T3 &max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    template<class T>
    static T Abs(const T &value)
    {
        return value < 0 ? -value : value;
    }

    template<class T = int, class T2>
    static T Round(const T2 &value)
    {
        return std::round(value);
    }

    template<class T>
    static T Ceil(const T &value)
    {
        return std::ceil(value);
    }

    template<class T>
    static T Floor(const T &value)
    {
        return std::floor(value);
    }

    template<class T>
    static T Fract(const T &value)
    {
        return value - Math::Floor(value);
    }


    template<class T>
    static T Pow(const T &base, const T &exponent)
    {
        return std::pow(base, exponent);
    }

    template<class T>
    static T Log(const T &value)
    {
        return std::log(value);
    }

    template<class T>
    static T Log10(const T &value)
    {
        return std::log10(value);
    }

    template<class T>
    static T Exp(const T &x)
    {
        return std::exp(x);
    }

    template<class T>
    static T Sqrt(const T &x)
    {
        return std::sqrt(x);
    }

    template<class T>
    static T Sin(const T &rad)
    {
        return std::sin(rad);
    }

    template<class T>
    static T Cos(const T &rad)
    {
        return std::cos(rad);
    }

    template<class T>
    static T Tan(const T &rad)
    {
        return std::tan(rad);
    }

    template<class T>
    static T Sinh(const T &rad)
    {
        return std::sinh(rad);
    }

    template<class T>
    static T Cosh(const T &rad)
    {
        return std::cosh(rad);
    }

    template<class T>
    static T Tanh(const T &rad)
    {
        return std::tanh(rad);
    }

    template<class T>
    static T ASin(const T &value)
    {
        return std::asin(value);
    }

    template<class T>
    static T ACos(const T &value)
    {
        return std::acos(value);
    }

    template<class T>
    static T ATan(const T &value)
    {
        return std::atan(value);
    }

    template<class T>
    static T ASinh(const T &value)
    {
        return std::asinh(value);
    }

    template<class T>
    static T ACosh(const T &value)
    {
        return std::acosh(value);
    }

    template<class T>
    static T ATanh(const T &value)
    {
        return std::atanh(value);
    }

    template<class T>
    static T ATan2(const T &valueX, const T &valueY)
    {
        return std::atan2(valueX, valueY);
    }

    template<class T>
    static T Modf(const T &value, const T &length)
    {
        return std::fmod(value, length);
    }

    template<class T>
    static T Sign(const T &value)
    {
        return static_cast<T>(value < 0 ? -1 : 1);
    }

    template<class T>
    static T Lerp(const T &a, const T &b, const T &t)
    {
        return (b-a) * t + a;
    }

    static float Map(float value,
                     float srcMin, float srcMax,
                     float destMin, float destMax)
    {
        return ((value-srcMin) / (srcMax - srcMin)) *
                (destMax - destMin) +
                destMin;
    }

    static float Deg2Rad(float deg)
    {
        return deg * static_cast<float>(Math::s_Deg2Rad);
    }

    static float Rad2Deg(float rad)
    {
		return rad * static_cast<float>(Math::s_Rad2Deg);
    }

    static double Deg2Rad(double deg)
    {
        return deg * Math::s_Deg2Rad;
    }

    static double Rad2Deg(double rad)
    {
        return rad * Math::s_Rad2Deg;
    }


private:

    static constexpr double s_Rad2Deg  = 180.0 / Math::PI;
    static constexpr double s_Deg2Rad = Math::PI / 180.0;

    Math() {}
};

NAMESPACE_BANG_END

#endif // MATH_H
