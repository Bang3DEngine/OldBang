#ifndef RECT_H
#define RECT_H

#include "Bang/Array.h"
#include "Bang/Vector2.h"
#include "Bang/Vector4.h"

NAMESPACE_BANG_BEGIN

template<class T>
class RectG
{
public:
    const static RectG<T> ScreenRect;
    const static RectG<T> Zero;

    Vector2G<T> m_min = Vector2G<T>::Zero;
    Vector2G<T> m_max = Vector2G<T>::Zero;

    RectG()
    {
    }

    template<class OtherT>
    explicit RectG(const RectG<OtherT> &r)
    {
        m_min = Vector2G<T>(r.GetMin());
        m_max = Vector2G<T>(r.GetMax());
    }

    explicit RectG(T minx, T miny, T maxx, T maxy)
    {
        m_min = Vector2G<T>( Math::Min(minx, maxx), Math::Min(miny, maxy) );
        m_max = Vector2G<T>( Math::Max(minx, maxx), Math::Max(miny, maxy) );
    }

    explicit RectG(const Vector2G<T> &p1, const Vector2G<T> &p2)
    {
        m_min = Vector2G<T>::Min(p1, p2);
        m_max = Vector2G<T>::Max(p1, p2);
    }

    void SetMin(const Vector2G<T> &minv) { m_min = minv; }
    void SetMax(const Vector2G<T> &maxv) { m_max = maxv; }

    Vector2G<T> GetMinXMaxY() const
    {
        return Vector2G<T>(m_min.x, m_max.y);
    }

    Vector2G<T> GetMaxXMaxY() const
    {
        return Vector2G<T>(m_max.x, m_max.y);
    }

    Vector2G<T> GetMinXMinY() const
    {
        return Vector2G<T>(m_min.x, m_min.y);
    }

    Vector2G<T> GetMaxXMinY() const
    {
        return Vector2G<T>(m_max.x, m_min.y);
    }

    const Vector2G<T>& GetMin() const
    {
        return m_min;
    }

    const Vector2G<T>& GetMax() const
    {
        return m_max;
    }

    T GetWidth() const
    {
        return (m_max.x - m_min.x);
    }

    T GetHeight() const
    {
        return (m_max.y - m_min.y);
    }

    T GetArea() const
    {
        return GetWidth() * GetHeight();
    }

    Vector2G<T> GetSize() const
    {
        return Vector2G<T>(GetWidth(), GetHeight());
    }

    Vector2G<T> GetCenter() const
    {
        return (GetMin() + GetMax()) / SCAST<T>(2);
    }

    bool Contains(const Vector2G<T> &p) const
    {
        return p.x >= m_min.x && p.x <= m_max.x &&
               p.y >= m_min.y && p.y <= m_max.y;
    }

    static RectG<T> Union(const RectG<T> &r1, const RectG<T> &r2)
    {
        return RectG<T>(Math::Min(r1.m_min.x, r2.m_min.x),
                        Math::Min(r1.m_min.y, r2.m_min.y),
                        Math::Max(r1.m_max.x, r2.m_max.x),
                        Math::Max(r1.m_max.y, r2.m_max.y));
    }

    static RectG<T> Intersection(const RectG<T> &r1, const RectG<T> &r2)
    {
        T minx = Math::Max(r1.m_min.x, r2.m_min.x);
        T miny = Math::Max(r1.m_min.y, r2.m_min.y);
        T maxx = Math::Min(r1.m_max.x, r2.m_max.x);
        T maxy = Math::Min(r1.m_max.y, r2.m_max.y);

        if (minx > maxx || miny > maxy)
        {
            return RectG<T>::Zero;
        }

        return RectG<T>(minx, miny, maxx, maxy);
    }

    template <class Iterator>
    static RectG<T> GetBoundingRectFromPositions(Iterator begin, Iterator end)
    {
        if (begin == end) { return RectG<T>::Zero; }

        Vector2 minv = *begin, maxv = *begin;
        for (auto it = begin; it != end; ++it)
        {
            const Vector2 &p = *it;
            minv = Vector2::Min(p, minv);
            maxv = Vector2::Max(p, maxv);
        }
        return RectG<T>(minv, maxv);
    }
};

template<class T>
const RectG<T> RectG<T>::ScreenRect = RectG<T>(Vector2G<T>(-1),
                                               Vector2G<T>(1));

template<class T>
const RectG<T> RectG<T>::Zero = RectG<T>(0, 0, 0, 0);

template<class T>
bool operator==(const RectG<T> &r1, const RectG<T> &r2)
{
    return r1.GetMin() == r2.GetMin() &&
           r1.GetMax() == r2.GetMax();
}


template<class T>
bool operator!=(const RectG<T> &r1, const RectG<T> &r2)
{
    return !(r1 == r2);
}

template<class T>
void operator*=(RectG<T> &r, T a)
{
    r.m_min *= a;
    r.m_max *= a;
}

template<class T>
void operator/=(RectG<T> &r, T a)
{
    r.m_min /= a;
    r.m_max /= a;
}

template<class T>
void operator*=(RectG<T> &r, const Vector2G<T> &v)
{
    r.m_min *= v;
    r.m_max *= v;
}

template<class T>
void operator/=(RectG<T> &r, const Vector2G<T> &v)
{
    r.m_min /= v;
    r.m_max /= v;
}

template<class T>
RectG<T> operator*(const Matrix4G<T> &m, const RectG<T> &r)
{
    return RectG<T>( (m * Vector4G<T>(r.GetMin(), 0, 1) ).xy(),
                     (m * Vector4G<T>(r.GetMax(), 0, 1) ).xy() );
}

template<class T>
RectG<T> operator/(T a, const RectG<T> &r)
{
    return RectG<T>(a / r.GetMin(), a / r.GetMax());
}

template<class T>
RectG<T> operator/(const RectG<T> &r, T a)
{
    return RectG<T>(r.GetMin() / a, r.GetMax() / a);
}

template<class T>
RectG<T> operator*(T a, const RectG<T> &r)
{
    return RectG<T>(a * r.GetMin(), a * r.GetMax());
}

template<class T>
RectG<T> operator*(const RectG<T> &r, T a)
{
    return a * r;
}

template<class T>
RectG<T> operator*(const Vector2G<T> &v, const RectG<T> &r)
{
    return RectG<T>(v * r.GetMin(), v * r.GetMax());
}

template<class T>
RectG<T> operator*(const RectG<T> &r, const Vector2G<T> &v)
{
    return v * r;
}

template<class T>
RectG<T> operator/(const Vector2G<T> &v, const RectG<T> &r)
{
    return RectG<T>(v / r.GetMin(), v / v.GetMax());
}

template<class T>
RectG<T> operator/(const RectG<T> &r, const Vector2G<T> &v)
{
    return RectG<T>(r.GetMin() / v, r.GetMax() / v);
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
    return RectG<T>(v - r.GetMin(), v - r.GetMax());
}

template<class T>
RectG<T> operator-(const RectG<T> &r, const Vector2G<T> &v)
{
    return RectG<T>(r.GetMin() - v, r.GetMax() - v);
}

template<class T>
void operator-=(RectG<T> &r, const Vector2G<T> &v)
{
    r.m_min = r.m_min - v;
    r.m_max = r.m_max - v;
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
    r.m_min += v;
    r.m_max += v;
}

NAMESPACE_BANG_END

#endif // RECT_H
