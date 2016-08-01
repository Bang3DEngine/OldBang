#include "Box.h"

#include "Logger.h"

Box::Box()
{
}

Box::Box(float minx, float maxx,
         float miny, float maxy,
         float minz, float maxz) :

    Box(Vector3(minx, miny, minz),
        Vector3(maxx, maxy, maxz))
{
}

Box::Box(const Vector3 &min, const Vector3 &max)
{
    this->m_minv = min;
    this->m_maxv = max;
}

Box::Box(const Box &b)
{
    m_minv = b.m_minv;
    m_maxv = b.m_maxv;
}

Vector3 Box::GetMin() const
{
    return m_minv;
}

Vector3 Box::GetMax() const
{
    return m_maxv;
}

Vector3 Box::GetDiagonal() const
{
    return m_maxv - m_minv;
}

float Box::GetWidth() const
{
    return (m_maxv.x - m_minv.x);
}

float Box::GetHeight() const
{
    return (m_maxv.y - m_minv.y);
}

float Box::GetDepth() const
{
    return (m_maxv.z - m_minv.z);
}

Vector3 Box::GetCenter() const
{
    return (m_minv + m_maxv) / 2.0f;
}

Vector3 Box::GetDimensions() const
{
    return Vector3(GetWidth(), GetHeight(), GetDepth());
}

float Box::GetArea() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * 2 +
           w * d * 2 +
           h * d * 2;
}

float Box::GetVolume() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * d;
}

Box Box::Union(const Box &b1, const Box &b2)
{
    return
    Box(std::min(b1.m_minv.x, b2.m_minv.x), std::max(b1.m_maxv.x, b2.m_maxv.x),
        std::min(b1.m_minv.y, b2.m_minv.y), std::max(b1.m_maxv.y, b2.m_maxv.y),
        std::min(b1.m_minv.z, b2.m_minv.z), std::max(b1.m_maxv.z, b2.m_maxv.z));
}

void Box::FillFromPositions(const std::vector<Vector3> &positions)
{
    m_minv = m_maxv = Vector3::zero;
    for (const Vector3 &v : positions)
    {
        m_minv.x = std::min(m_minv.x, v.x);
        m_maxv.x = std::max(m_maxv.x, v.x);

        m_minv.y = std::min(m_minv.y, v.y);
        m_maxv.y = std::max(m_maxv.y, v.y);

        m_minv.z = std::min(m_minv.z, v.z);
        m_maxv.z = std::max(m_maxv.z, v.z);
    }
}


Box operator*(const Matrix4 &m, const Box &b)
{
    Vector3 minv = b.GetMin();
    Vector3 maxv = b.GetMax();
    glm::vec4 bMinCoords = glm::vec4(minv.x, minv.y, minv.z, 1.0f);
    glm::vec4 bMaxCoords = glm::vec4(maxv.x, maxv.y, maxv.z, 1.0f);
    glm::vec4 v1 = m * bMinCoords;
    glm::vec4 v2 = m * bMaxCoords;

    Box br(std::min(v1.x, v2.x), std::max(v1.x, v2.x),
           std::min(v1.y, v2.y), std::max(v1.y, v2.y),
           std::min(v1.z, v2.z), std::max(v1.z, v2.z));
    return br;
}



const std::string Box::ToString() const
{
    std::ostringstream oss;
    oss << "Box: [" << std::endl <<
           "  min: " << m_minv << std::endl <<
           "  max" << m_maxv << std::endl <<
           "]" << std::endl;

    return oss.str();
}
