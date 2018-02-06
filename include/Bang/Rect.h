#ifndef RECT_H
#define RECT_H

#include <array>

#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

template <class T>
class RectG
{
private:
    Vector2G<T> m_corner;
    std::array<Vector2G<T>, 2> m_extents;

    static constexpr float ChecksEpsilon = 0.0001f;

public:
    const static RectG<T> NDCRect;
    const static RectG<T> Zero;

    RectG() {}
    RectG(const Vector2G<T> &corner,
          const Vector2G<T> &extent0,
          const T &extent1Size)
    {
        SetCorner(corner);
        SetExtents(extent0, extent1Size);
    }

    void SetCorner(const Vector2G<T> &corner)
    {
        m_corner = corner;
    }

    void SetExtents( const Vector2G<T> &extent0, const T &extent1Size)
    {
        m_extents[0] = extent0;
        m_extents[1] = extent0.Perpendicular().NormalizedSafe() * extent1Size;
    }

    const Vector2G<T>& GetCorner() const { return m_corner; }
    Vector2G<T> GetExtent(int i) const
    {
        ASSERT(i == 0 || i == 1);
        return m_extents[i];
    }

    std::array<Vector2G<T>, 4> GetPoints() const
    {
        return {GetCorner(),
                GetCorner() + GetExtent(0),
                GetCorner() + GetExtent(1),
                GetCorner() + GetExtent(0) + GetExtent(1)};
    }
};

template<class T>
const RectG<T> RectG<T>::NDCRect = RectG<T>(Vector2G<T>(-1),
                                            Vector2G<T>(1));

template<class T>
const RectG<T> RectG<T>::Zero = RectG<T>(0, 0, 0, 0);

NAMESPACE_BANG_END

#endif // RECT_H

