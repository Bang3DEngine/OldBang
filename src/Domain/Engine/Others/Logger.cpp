#include "Logger.h"

std::ostream& operator<<(std::ostream &log, const std::string &str)
{
    log << str.c_str();
    return log;
}

std::ostream& operator<<(std::ostream &log, const Vector2 &v)
{
    log << "(" << v.x << ", " << v.y << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const Vector3 &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const Vector4 &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const Quaternion &q)
{
    log << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return log;
}


std::ostream& operator<<(std::ostream &log, const Matrix4 &v)
{
    log << "(" << v[0][0] << ", " << v[1][0] << ", " << v[2][0] << ", " << v[3][0] << "," <<std::endl;
    log << " " << v[0][1] << ", " << v[1][1] << ", " << v[2][1] << ", " << v[3][1] << "," <<std::endl;
    log << " " << v[0][2] << ", " << v[1][2] << ", " << v[2][2] << ", " << v[3][2] << "," <<std::endl;
    log << " " << v[0][3] << ", " << v[1][3] << ", " << v[2][3] << ", " << v[3][3] << ")" <<std::endl;
    return log;
}

std::ostream &operator<<(std::ostream &log, const IToString *s)
{
    if (!s)
    {
        log << "Null";
        return log;
    }

    log << s->ToString();
    return log;
}


std::ostream &operator<<(std::ostream &log, const IToString &s)
{
    log << s.ToString();
    return log;
}


std::ostream &operator<<(std::ostream &log, const Color &c)
{
    log << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return log;
}
