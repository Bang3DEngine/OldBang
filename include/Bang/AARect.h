#ifndef AARECT_H
#define AARECT_H

#include "Bang/Array.h"
#include "Bang/Vector2.h"
#include "Bang/Vector4.h"

NAMESPACE_BANG_BEGIN

template<class T>
class AARectG
{
private:
    Vector2G<T> m_min = Vector2G<T>::Zero;
    Vector2G<T> m_max = Vector2G<T>::Zero;

public:
    const static AARectG<T> NDCRect;
    const static AARectG<T> Zero;

    AARectG()
    {
    }

    template<class OtherT>
    explicit AARectG(const AARectG<OtherT> &r)
    {
        m_min = Vector2G<T>(r.GetMin());
        m_max = Vector2G<T>(r.GetMax());
    }

    template<class OtherT>
    explicit AARectG(const RectG<OtherT> &r)
    {
        Vector2G<OtherT> p0, p1, opposedP0;
        r.GetPoints(&p0, &p1, &opposedP0);
        *this = AARectG(Vector2G<T>(p0), Vector2G<T>(opposedP0));
    }

    explicit AARectG(T minx, T miny, T maxx, T maxy)
    {
        m_min = Vector2G<T>( Math::Min(minx, maxx), Math::Min(miny, maxy) );
        m_max = Vector2G<T>( Math::Max(minx, maxx), Math::Max(miny, maxy) );
    }

    explicit AARectG(const Vector2G<T> &p1, const Vector2G<T> &p2)
    {
        m_min = Vector2G<T>::Min(p1, p2);
        m_max = Vector2G<T>::Max(p1, p2);
    }

    void SetMin(const Vector2G<T> &minv) { m_min = minv; }
    void SetMax(const Vector2G<T> &maxv) { m_max = maxv; }

    Vector2G<T> GetMinXMaxY() const
    {
        return Vector2G<T>(GetMin().x, GetMax().y);
    }

    Vector2G<T> GetMaxXMaxY() const
    {
        return Vector2G<T>(GetMax().x, GetMax().y);
    }

    Vector2G<T> GetMinXMinY() const
    {
        return Vector2G<T>(GetMin().x, GetMin().y);
    }

    Vector2G<T> GetMaxXMinY() const
    {
        return Vector2G<T>(GetMax().x, GetMin().y);
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
        return (GetMax().x - GetMin().x);
    }

    T GetHeight() const
    {
        return (GetMax().y - GetMin().y);
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
        return (GetMin() + GetMax()) / Cast<T>(2);
    }

    bool Contains(const Vector2G<T> &p) const
    {
        return p.x >= GetMin().x && p.x < GetMax().x &&
               p.y >= GetMin().y && p.y < GetMax().y;
    }

    static AARectG<T> Union(const AARectG<T> &r1, const AARectG<T> &r2)
    {
        return AARectG<T>(Math::Min(r1.GetMin().x, r2.GetMin().x),
                          Math::Min(r1.GetMin().y, r2.GetMin().y),
                          Math::Max(r1.GetMax().x, r2.GetMax().x),
                          Math::Max(r1.GetMax().y, r2.GetMax().y));
    }

    template <class Iterator>
    static AARectG<T> Union(Iterator begin, Iterator end)
    {
        if (begin == end) { return AARectG<T>::Zero; }
        AARectG<T> unionRect = *begin;
        for (auto it = begin; it != end; ++it)
        { unionRect = AARectG<T>::Union(unionRect, *it); }
        return unionRect;
    }

    static AARectG<T> Intersection(const AARectG<T> &r1, const AARectG<T> &r2)
    {
        T minx = Math::Max(r1.GetMin().x, r2.GetMin().x);
        T miny = Math::Max(r1.GetMin().y, r2.GetMin().y);
        T maxx = Math::Min(r1.GetMax().x, r2.GetMax().x);
        T maxy = Math::Min(r1.GetMax().y, r2.GetMax().y);

        if (minx > maxx || miny > maxy)
        {
            return AARectG<T>::Zero;
        }

        return AARectG<T>(minx, miny, maxx, maxy);
    }

    template <class Iterator>
    static AARectG<T> Intersection(Iterator begin, Iterator end)
    {
        if (begin == end) { return AARectG<T>::Zero; }
        AARectG<T> intersectionRect = *begin;
        for (auto it = begin; it != end; ++it)
        { intersectionRect = AARectG<T>::Intersection(intersectionRect, *it); }
        return intersectionRect;
    }

    template <class Iterator>
    static AARectG<T> GetBoundingRectFromPositions(Iterator begin, Iterator end)
    {
        if (begin == end) { return AARectG<T>::Zero; }

        Vector2G<T> minv = *begin, maxv = *begin;
        for (auto it = begin; it != end; ++it)
        {
            const Vector2G<T> &p = *it;
            minv = Vector2G<T>::Min(p, minv);
            maxv = Vector2G<T>::Max(p, maxv);
        }
        return AARectG<T>(minv, maxv);
    }

    template <class OtherT = T>
    RectG<OtherT> ToRect() const
    {
        return RectG<OtherT>(Vector2G<OtherT>(GetCenter()),
                             Vector2G<OtherT>::Right,
                             SCAST<OtherT>(GetWidth()  * 0.5),
                             SCAST<OtherT>(GetHeight() * 0.5));
    }

    template<class S> friend bool operator==(const AARectG<S> &r1, const AARectG<S> &r2);
    template<class S> friend bool operator!=(const AARectG<S> &r1, const AARectG<S> &r2);
    template<class S> friend void operator*=(AARectG<S> &r, S a);
    template<class S> friend void operator/=(AARectG<S> &r, S a);
    template<class S> friend void operator*=(AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator/=(AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend AARectG<S> operator*(const Matrix4G<S> &m, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator/(S a, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator/(const AARectG<S> &r, S a);
    template<class S> friend AARectG<S> operator*(S a, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator*(const AARectG<S> &r, S a);
    template<class S> friend AARectG<S> operator*(const Vector2G<S> &v, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator*(const AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend AARectG<S> operator/(const Vector2G<S> &v, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator/(const AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend AARectG<S> operator-(S a, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator-(const AARectG<S> &r, S a);
    template<class S> friend AARectG<S> operator-(const Vector2G<S> &v, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator-(const AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator-=(AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend AARectG<S> operator+(S a, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator+(const AARectG<S> &r, S a);
    template<class S> friend AARectG<S> operator+(const Vector2G<S> &v, const AARectG<S> &r);
    template<class S> friend AARectG<S> operator+(const AARectG<S> &r, const Vector2G<S> &v);
    template<class S> friend void operator+=(AARectG<S> &r, const Vector2G<S> &v);
};

template<class T>
const AARectG<T> AARectG<T>::NDCRect = AARectG<T>(Vector2G<T>(-1),
                                               Vector2G<T>(1));

template<class T>
const AARectG<T> AARectG<T>::Zero = AARectG<T>(0, 0, 0, 0);

template<class T>
bool operator==(const AARectG<T> &r1, const AARectG<T> &r2)
{
    return r1.GetMin() == r2.GetMin() &&
           r1.GetMax() == r2.GetMax();
}


template<class T>
bool operator!=(const AARectG<T> &r1, const AARectG<T> &r2)
{
    return !(r1 == r2);
}

template<class T>
void operator*=(AARectG<T> &r, T a)
{
    r.m_min *= a;
    r.m_max *= a;
}

template<class T>
void operator/=(AARectG<T> &r, T a)
{
    r.m_min /= a;
    r.m_max /= a;
}

template<class T>
void operator*=(AARectG<T> &r, const Vector2G<T> &v)
{
    r.m_min *= v;
    r.m_max *= v;
}

template<class T>
void operator/=(AARectG<T> &r, const Vector2G<T> &v)
{
    r.m_min /= v;
    r.m_max /= v;
}

template<class T>
AARectG<T> operator*(const Matrix4G<T> &m, const AARectG<T> &r)
{
    return AARectG<T>( (m * Vector4G<T>(r.GetMin(), 0, 1) ).xy(),
                       (m * Vector4G<T>(r.GetMax(), 0, 1) ).xy() );
}

template<class T>
AARectG<T> operator/(T a, const AARectG<T> &r)
{
    return AARectG<T>(a / r.GetMin(), a / r.GetMax());
}

template<class T>
AARectG<T> operator/(const AARectG<T> &r, T a)
{
    return AARectG<T>(r.GetMin() / a, r.GetMax() / a);
}

template<class T>
AARectG<T> operator*(T a, const AARectG<T> &r)
{
    return AARectG<T>(a * r.GetMin(), a * r.GetMax());
}

template<class T>
AARectG<T> operator*(const AARectG<T> &r, T a)
{
    return a * r;
}

template<class T>
AARectG<T> operator*(const Vector2G<T> &v, const AARectG<T> &r)
{
    return AARectG<T>(v * r.GetMin(), v * r.GetMax());
}

template<class T>
AARectG<T> operator*(const AARectG<T> &r, const Vector2G<T> &v)
{
    return v * r;
}

template<class T>
AARectG<T> operator/(const Vector2G<T> &v, const AARectG<T> &r)
{
    return AARectG<T>(v / r.GetMin(), v / v.GetMax());
}

template<class T>
AARectG<T> operator/(const AARectG<T> &r, const Vector2G<T> &v)
{
    return AARectG<T>(r.GetMin() / v, r.GetMax() / v);
}

template<class T>
AARectG<T> operator-(T a, const AARectG<T> &r)
{
    return Vector2G<T>(a) - r;
}

template<class T>
AARectG<T> operator-(const AARectG<T> &r, T a)
{
    return r - Vector2G<T>(a);
}

template<class T>
AARectG<T> operator-(const Vector2G<T> &v, const AARectG<T> &r)
{
    return AARectG<T>(v - r.GetMin(), v - r.GetMax());
}

template<class T>
AARectG<T> operator-(const AARectG<T> &r, const Vector2G<T> &v)
{
    return AARectG<T>(r.GetMin() - v, r.GetMax() - v);
}

template<class T>
void operator-=(AARectG<T> &r, const Vector2G<T> &v)
{
    r.m_min = r.m_min - v;
    r.m_max = r.m_max - v;
}

template<class T>
AARectG<T> operator+(T a, const AARectG<T> &r)
{
    return Vector2G<T>(a) + r;
}

template<class T>
AARectG<T> operator+(const AARectG<T> &r, T a)
{
    return Vector2G<T>(a) + r;
}

template<class T>
AARectG<T> operator+(const Vector2G<T> &v, const AARectG<T> &r)
{
    AARectG<T> res = r;
    res += v;
    return res;
}

template<class T>
AARectG<T> operator+(const AARectG<T> &r, const Vector2G<T> &v)
{
    return v + r;
}

template<class T>
void operator+=(AARectG<T> &r, const Vector2G<T> &v)
{
    r.m_min += v;
    r.m_max += v;
}

NAMESPACE_BANG_END

#endif // AARECT_H
