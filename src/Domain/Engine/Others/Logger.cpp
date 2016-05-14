#include "Logger.h"

std::ostream& operator<<(std::ostream &log, const std::string &str)
{
    log << str.c_str();
    return log;
}

std::ostream& operator<<(std::ostream &log, const glm::vec2 &v)
{
    log << "(" << v.x << ", " << v.y << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const glm::vec3 &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const glm::vec4 &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const glm::quat &q)
{
    log << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return log;
}


std::ostream& operator<<(std::ostream &log, const glm::mat4 &v)
{
    log << "(" << v[0][0] << ", " << v[1][0] << ", " << v[2][0] << ", " << v[3][0] << "," <<std::endl;
    log << " " << v[0][1] << ", " << v[1][1] << ", " << v[2][1] << ", " << v[3][1] << "," <<std::endl;
    log << " " << v[0][2] << ", " << v[1][2] << ", " << v[2][2] << ", " << v[3][2] << "," <<std::endl;
    log << " " << v[0][3] << ", " << v[1][3] << ", " << v[2][3] << ", " << v[3][3] << ")" <<std::endl;
    return log;
}

std::ostream &operator<<(std::ostream &log, const IToString *s)
{
    if(s == nullptr)
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
