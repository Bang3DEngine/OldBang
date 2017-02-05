#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <limits>

class Math
{
public:
    static constexpr double PI = 3.1415926535897932384626;

    template<class T>
    static const T& Min()
    {
        return std::numeric_limits<T>::min();
    }

    template<class T>
    static const T& Max()
    {
        return std::numeric_limits<T>::max();
    }

    template<class T>
    static const T& Infinity()
    {
        return std::numeric_limits<T>::infinity();
    }

    template<class T>
    static const T& NegativeInfinity()
    {
        return -std::numeric_limits<T>::infinity();
    }

    template<class T>
    static const T& IsInfinity(const T& value)
    {
        return std::isinf(value);
    }

    template<class T>
    static const T& IsNaN(const T& value)
    {
        return std::isnan(value);
    }

    template<class T>
    static const T& Min(const T &a, const T &b)
    {
        return (a <= b) ? a : b;
    }

    template<class T>
    static const T& Max(const T &a, const T &b)
    {
        return (a >= b) ? a : b;
    }

    template<class T>
    static const T& Clamp(const T &value, const T &min, const T &max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    template<class T>
    static const T& Abs(const T &value)
    {
        return value < 0 ? -value : value;
    }

    template<class T>
    static const T& Round(const T &value)
    {
        return std::round(value);
    }

    template<class T>
    static const T& Ceil(const T &value)
    {
        return std::ceil(value);
    }

    template<class T>
    static const T& Floor(const T &value)
    {
        return std::floor(value);
    }

    template<class T>
    static const T& Pow(const T &base, const T &exponent)
    {
        return std::pow(base, exponent);
    }

    template<class T>
    static const T& Log(const T &value)
    {
        return std::log(value);
    }

    template<class T>
    static const T& Log10(const T &value)
    {
        return std::log10(value);
    }

    template<class T>
    static const T& Exp(const T &x)
    {
        return std::exp(x);
    }

    template<class T>
    static const T& Sqrt(const T &x)
    {
        return std::sqrt(x);
    }

    template<class T>
    static const T& Sin(const T &rad)
    {
        return std::sin(rad);
    }

    template<class T>
    static const T& Cos(const T &rad)
    {
        return std::cos(rad);
    }

    template<class T>
    static const T& Tan(const T &rad)
    {
        return std::tan(rad);
    }

    template<class T>
    static const T& Sinh(const T &rad)
    {
        return std::sinh(rad);
    }

    template<class T>
    static const T& Cosh(const T &rad)
    {
        return std::cosh(rad);
    }

    template<class T>
    static const T& Tanh(const T &rad)
    {
        return std::tanh(rad);
    }

    template<class T>
    static const T& ASin(const T &value)
    {
        return std::asin(value);
    }

    template<class T>
    static const T& ACos(const T &value)
    {
        return std::acos(value);
    }

    template<class T>
    static const T& ATan(const T &value)
    {
        return std::atan(value);
    }

    template<class T>
    static const T& ASinh(const T &value)
    {
        return std::asinh(value);
    }

    template<class T>
    static const T& ACosh(const T &value)
    {
        return std::acosh(value);
    }

    template<class T>
    static const T& ATanh(const T &value)
    {
        return std::atanh(value);
    }

    template<class T>
    static const T& ATan2(const T &valueX, const T &valueY)
    {
        return std::atan2(valueX, valueY);
    }

    template<class T>
    static const T& Modf(const T &value, const T& length)
    {
        return std::modf(value, length);
    }

    template<class T>
    static const T& Sign(const T &value)
    {
        return static_cast<T>(value < 0 ? -1 : 1);
    }

    template<class T>
    static const T& Lerp(const T &a, const T &b, const T& t)
    {
        return (b-a) * t + a;
    }

    static float Deg2Rad(float deg)
    {
        return deg * Math::s_Deg2Rad;
    }

    static float Rad2Deg(float rad)
    {
        return rad * Math::s_Rad2Deg;
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

#endif // MATH_H
