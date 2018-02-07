#ifndef RECT_H
#define RECT_H

#include <array>

#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

template <class T>
class RectG
{
private:
    Vector2G<T> m_center;
    Vector2G<T> m_axis0;
    T m_halfSizeAxis0;
    T m_halfSizeAxis1;

    static constexpr float ChecksEpsilon = 0.0001f;

public:
    const static RectG<T> NDCRect;
    const static RectG<T> Zero;

    RectG() {}

    RectG(const Vector2G<T> &center,
          const Vector2G<T> &axis0,
          const T &halfSizeAxis0,
          const T &halfSizeAxis1)
    {
        SetCenter(center);
        SetAxis(axis0);
        SetHalfSizes(halfSizeAxis0, halfSizeAxis1);
    }

    template<class OtherT>
    explicit RectG(const RectG<OtherT> &r)
    {
        SetCenter(Vector2G<T>(r.GetCenter()));
        SetAxis(Vector2G<T>(r.GetAxis(0)));
        SetHalfSizes(SCAST<T>(r.GetHalfSize(0)), SCAST<T>(r.GetHalfSize(1)));
    }

    RectG(const Vector2G<T> &center,
          const Vector2G<T> &axis0,
          const Vector2G<T> &halfSizeAxis) :
        RectG(center, axis0, halfSizeAxis.x, halfSizeAxis.y)
    {
    }

    void SetCenter(const Vector2G<T> &center)
    {
        m_center = center;
    }

    void SetAxis(const Vector2G<T> &axis0)
    {
        m_axis0 = axis0.NormalizedSafe();
    }

    void SetHalfSize(int i, const T &halfSize)
    {
        if (i == 0) { m_halfSizeAxis0 = halfSize; }
        else if (i == 1) { m_halfSizeAxis1 = halfSize; }
    }

    void SetHalfSizes(const T &halfSizeAxis0, const T &halfSizeAxis1)
    {
        SetHalfSize(0, halfSizeAxis0);
        SetHalfSize(1, halfSizeAxis1);
    }

    const Vector2G<T>& GetCenter() const { return m_center; }
    Vector2G<T> GetAxis(int i) const
    {
        ASSERT(i == 0 || i == 1);
        if (i == 0) { return m_axis0; }
        return m_axis0.Perpendicular();
    }

    T GetHalfSize(int i) const
    {
        ASSERT(i == 0 || i == 1);
        if (i == 0) { return m_halfSizeAxis0; }
        return m_halfSizeAxis1;
    }

    Vector2G<T> GetHalfExtent(int i) const
    {
        return GetAxis(i) * GetHalfSize(i);
    }

    Vector2G<T> GetHalfSize() const
    {
        return Vector2G<T>(GetHalfSize(0), GetHalfSize(1));
    }
    Vector2G<T> GetSize() const
    {
        return GetHalfSize() * T(2);
    }

    bool Contains(const Vector2G<T> &point) const
    {
        Vector2G<T> a, b, c, d;
        GetPoints(&a, &b, &c, &d);

        const Vector2G<T> e = (b - a);
        const Vector2G<T> f = (d - a);
        if ((point.x - a.x)*e.x + (point.y - a.y)*e.y <= 0.0) { return false; }
        if ((point.x - b.x)*e.x + (point.y - b.y)*e.y >= 0.0) { return false; }
        if ((point.x - a.x)*f.x + (point.y - a.y)*f.y <= 0.0) { return false; }
        if ((point.x - d.x)*f.x + (point.y - d.y)*f.y >= 0.0) { return false; }

        return true;
    }

    void GetPoints(Vector2G<T> *p0,
                   Vector2G<T> *p1,
                   Vector2G<T> *opposedP0) const
    {
        *p0        = GetCenter() + Vector2G<T>(-GetHalfSize(0), -GetHalfSize(1));
        *p1        = GetCenter() + Vector2G<T>(-GetHalfSize(0), +GetHalfSize(1));
        *opposedP0 = GetCenter() + Vector2G<T>(+GetHalfSize(0), +GetHalfSize(1));
    }
    void GetPoints(Vector2G<T> *p0,
                   Vector2G<T> *p1,
                   Vector2G<T> *opposedP0,
                   Vector2G<T> *opposedP1) const
    {
        GetPoints(p0, p1, opposedP0);
        *opposedP1 = GetCenter() + Vector2G<T>(+GetHalfSize(0), -GetHalfSize(1));
    }

    std::array<Vector2G<T>, 4> GetPoints() const
    {
        Vector2G<T> p0, p1, p2, p3;
        GetPoints(&p0, &p1, &p2, &p3);
        return {p0, p1, p2, p3};
    }

    template<class S> friend bool operator==(const RectG<S> &r1, const RectG<S> &r2);
    template<class S> friend bool operator!=(const RectG<S> &r1, const RectG<S> &r2);
    template<class S> friend void operator*=(RectG<S> &r, S a);
    template<class S> friend void operator/=(RectG<S> &r, S a);
    template<class S> friend void operator*=(RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator/=(RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend RectG<S> operator*(const Matrix4G<S> &m, const RectG<S> &r);
    template<class S> friend RectG<S> operator/(S a, const RectG<S> &r);
    template<class S> friend RectG<S> operator/(const RectG<S> &r, S a);
    template<class S> friend RectG<S> operator*(S a, const RectG<S> &r);
    template<class S> friend RectG<S> operator*(const RectG<S> &r, S a);
    template<class S> friend RectG<S> operator*(const Vector2G<S> &v, const RectG<S> &r);
    template<class S> friend RectG<S> operator*(const RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend RectG<S> operator/(const Vector2G<S> &v, const RectG<S> &r);
    template<class S> friend RectG<S> operator/(const RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend RectG<S> operator-(S a, const RectG<S> &r);
    template<class S> friend RectG<S> operator-(const RectG<S> &r, S a);
    template<class S> friend RectG<S> operator-(const Vector2G<S> &v, const RectG<S> &r);
    template<class S> friend RectG<S> operator-(const RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator-=(RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend RectG<S> operator+(S a, const RectG<S> &r);
    template<class S> friend RectG<S> operator+(const RectG<S> &r, S a);
    template<class S> friend RectG<S> operator+(const Vector2G<S> &v, const RectG<S> &r);
    template<class S> friend RectG<S> operator+(const RectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator+=(RectG<S> &r, const Vector2G<S> &v);
};

template<class T>
const RectG<T> RectG<T>::NDCRect = RectG<T>(Vector2G<T>(0),
                                            Vector2G<T>(1, 0),
                                            1, 1);

template<class T>
const RectG<T> RectG<T>::Zero = RectG<T>(Vector2G<T>(0), Vector2G<T>(0), 0, 0);

template<class T>
bool operator==(const RectG<T> &r1, const RectG<T> &r2)
{
    return r1.GetCorner()   == r2.GetCorner() &&
           r1.GetAxis0()    == r2.GetAxis0() &&
           r1.GetHalfSize() == r2.GetHalfSize();
}


template<class T>
bool operator!=(const RectG<T> &r1, const RectG<T> &r2)
{
    return !(r1 == r2);
}

template<class T>
void operator*=(RectG<T> &r, T a)
{
    r.m_center    *= a;
    r.m_halfSize0 *= a;
    r.m_halfSize1 *= a;
}

template<class T>
void operator/=(RectG<T> &r, T a)
{
    r.m_center    /= a;
    r.m_halfSize0 /= a;
    r.m_halfSize1 /= a;
}

template<class T>
void operator*=(RectG<T> &r, const Vector2G<T> &v)
{
    r.m_center    *= v;
    r.m_halfSize0 *= v[0];
    r.m_halfSize1 *= v[1];
}

template<class T>
void operator/=(RectG<T> &r, const Vector2G<T> &v)
{
    r.m_center    /= v;
    r.m_halfSize0 /= v[0];
    r.m_halfSize1 /= v[1];
}

template<class T>
RectG<T> operator*(const Matrix4G<T> &m, const RectG<T> &r)
{
    return RectG<T>(
       (m * Vector4G<T>(r.GetCenter(),      0, 1)).xy(),
       (m * Vector4G<T>(r.GetAxis(0),       0, 0)).xy(),
       (m * Vector4G<T>(r.GetHalfExtent(0), 0, 0)).xy().Length(),
       (m * Vector4G<T>(r.GetHalfExtent(1), 0, 0)).xy().Length() );
}

template<class T>
RectG<T> operator/(T a, const RectG<T> &r)
{
    return RectG<T>(a / r.GetCorner(),
                    r.GetAxis(0),
                    a / r.GetHalfSize(0),
                    a / r.GetHalfSize(1));
}

template<class T>
RectG<T> operator/(const RectG<T> &r, T a)
{
    return RectG<T>(r.GetCorner() / a,
                    r.GetAxis(0),
                    r.GetHalfSize(0) / a,
                    r.GetHalfSize(1) / a);
}

template<class T>
RectG<T> operator*(T a, const RectG<T> &r)
{
    return RectG<T>(a * r.GetCorner(),
                    r.GetAxis(0),
                    a * r.GetHalfSize(0),
                    a * r.GetHalfSize(1));
}

template<class T>
RectG<T> operator*(const RectG<T> &r, T a)
{
    return a * r;
}

template<class T>
RectG<T> operator*(const Vector2G<T> &v, const RectG<T> &r)
{
    return RectG<T>(v * r.GetCorner(),
                    r.GetAxis(0),
                    v[0] * r.GetHalfSize(0),
                    v[1] * r.GetHalfSize(1));
}

template<class T>
RectG<T> operator*(const RectG<T> &r, const Vector2G<T> &v)
{
    return v * r;
}

template<class T>
RectG<T> operator/(const Vector2G<T> &v, const RectG<T> &r)
{
    return RectG<T>(v / r.GetCorner(),
                    r.GetAxis(0),
                    v[0] / r.GetHalfSize(0),
                    v[1] / r.GetHalfSize(1));
}

template<class T>
RectG<T> operator/(const RectG<T> &r, const Vector2G<T> &v)
{
    return RectG<T>(r.GetCorner() / v,
                    r.GetAxis(0),
                    r.GetHalfSize(0) / v[0],
                    r.GetHalfSize(1) / v[1]);
}

template<class T>
RectG<T> operator-(T a, const RectG<T> &r)
{
    return Vector2G<T>(a) - r;
}

template<class T>
RectG<T> operator-(const RectG<T> &r, T a)
{
    return r - Vector2G<T>(a);
}

template<class T>
RectG<T> operator-(const Vector2G<T> &v, const RectG<T> &r)
{
    return RectG<T>(v - r.GetCorner(),
                    r.GetAxis(0),
                    v[0] - r.GetHalfSize(0),
                    v[1] - r.GetHalfSize(1));
}

template<class T>
RectG<T> operator-(const RectG<T> &r, const Vector2G<T> &v)
{
    return RectG<T>(r.GetCorner() - v,
                    r.GetAxis(0),
                    r.GetHalfSize(0) - v[0],
                    r.GetHalfSize(1) - v[1]);
}

template<class T>
void operator-=(RectG<T> &r, const Vector2G<T> &v)
{
    r = r - v;
}

template<class T>
RectG<T> operator+(T a, const RectG<T> &r)
{
    return Vector2G<T>(a) + r;
}

template<class T>
RectG<T> operator+(const RectG<T> &r, T a)
{
    return Vector2G<T>(a) + r;
}

template<class T>
RectG<T> operator+(const Vector2G<T> &v, const RectG<T> &r)
{
    RectG<T> res = r;
    res += v;
    return res;
}

template<class T>
RectG<T> operator+(const RectG<T> &r, const Vector2G<T> &v)
{
    return v + r;
}

template<class T>
void operator+=(RectG<T> &r, const Vector2G<T> &v)
{
    r = r + v;
}


NAMESPACE_BANG_END

#endif // RECT_H

