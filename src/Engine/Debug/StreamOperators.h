#ifndef STREAMOPERATORS_H
#define STREAMOPERATORS_H

#include <sstream>
#include "Bang.h"

FORWARD_T  class Set;
FORWARD_TT class Map;
FORWARD    class Path;
FORWARD_T  class List;
FORWARD    class Color;
FORWARD_T  class Array;
FORWARD_T  class RectG;
FORWARD    class String;
FORWARD    class QString;
FORWARD_T  class Vector2G;
FORWARD_T  class Vector3G;
FORWARD_T  class Vector4G;
FORWARD_T  class Matrix3G;
FORWARD_T  class Matrix4G;
FORWARD    class IToString;
FORWARD_T  class QuaternionG;

std::istringstream& operator>>(std::istringstream& is, Path &p);
std::istringstream& operator>>(std::istringstream& is, Color &c);
std::ostream& operator<<(std::ostream &log, const Color &v);
std::ostream& operator<<(std::ostream &log, const IToString &v);
std::ostream& operator<<(std::ostream &log, const QString &str);
std::ostream& operator<<(std::ostream &log, const IToString *s);

// Templated ostream operators
template<class T>
std::ostream& operator<<(std::ostream &log, const Vector2G<T> &v)
{
    log << "(" << v.x << ", " << v.y << ")";
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const Vector3G<T> &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const Vector4G<T> &v)
{
    log << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const QuaternionG<T> &q)
{
    log << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const RectG<T> &r)
{
    log << "(" << r.GetMin() << ", " <<
                  r.GetMax() << ")";
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const Matrix3G<T> &m)
{
    log << "(" << m.c0[0] << ", " << m.c1[0] << ", " << m.c2[0] << "," << std::endl;
    log << " " << m.c0[1] << ", " << m.c1[1] << ", " << m.c2[1] << "," << std::endl;
    log << " " << m.c0[2] << ", " << m.c1[2] << ", " << m.c2[2] << "," << std::endl;
    log << " " << m.c0[3] << ", " << m.c1[3] << ", " << m.c2[3] << ")" << std::endl;
    return log;
}

template<class T>
std::ostream& operator<<(std::ostream &log, const Matrix4G<T> &m)
{
    log << "(" << m.c0[0] << ", " << m.c1[0] << ", " << m.c2[0] << ", " << m.c3[0] << "," << std::endl;
    log << " " << m.c0[1] << ", " << m.c1[1] << ", " << m.c2[1] << ", " << m.c3[1] << "," << std::endl;
    log << " " << m.c0[2] << ", " << m.c1[2] << ", " << m.c2[2] << ", " << m.c3[2] << "," << std::endl;
    log << " " << m.c0[3] << ", " << m.c1[3] << ", " << m.c2[3] << ", " << m.c3[3] << ")" << std::endl;
    return log;
}

// List
template <class T>
std::ostream &operator<<(std::ostream &log, const List<T> *l)
{
    log << "("; bool first = true;
    for (auto it = l->Begin(); it != l->End(); ++it)
    {
        if (!first) log << ", ";
        log << (*it);
        first = false;
    }
    log <<")";
    return log;
}

template <class T, class M>
std::ostream &operator<<(std::ostream &log, const std::pair<T,M>& p)
{
    log << "<";
    log << p.first << ", " << p.second;
    log <<">";
    return log;
}

template <class T>
std::ostream &operator<<(std::ostream &log, const List<T> &l)
{
    return log << (&l);
}

// Vector
template <class T>
std::ostream &operator<<(std::ostream &log, const Array<T> *v)
{
    log << "[";
    for (int i = 0; i < v->Size(); ++i)
    {
        if (i != 0) log << ", ";
        log << (*v)[i];
    }
    log << "]";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const Array<T> &v)
{
    return log << (&v);
}

// Map
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const Map<T,S> *m)
{
    log << "{";
    for (auto it = m->CBegin(); it != m->CEnd(); ++it)
    {
        if (it != m->cbegin()) log << ", ";
        log << (it->first) << ": " << (it->second);
    }
    log << "}";
    return log;
}
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const Map<T,S> &m)
{
    return log << (&m);
}

// Set
template <class T>
std::ostream &operator<<(std::ostream &log, const Set<T> *s)
{
    log << "{";
    for (auto it = s->cbegin(); it != s->cend(); ++it)
    {
        if (it != s->cbegin()) log << ", ";
        log << (*it);
    }
    log << "}";
    return log;
}

template <class T>
std::ostream &operator<<(std::ostream &log, const Set<T> &m)
{
    return log << (&m);
}

// Templated istream operators
template<class T>
std::istringstream& operator>>(std::istringstream &is, Vector2G<T>& v)
{
    char _;
    is >> _ >> v.x >> _ >> v.y >> _;
    return is;
}

template<class T>
std::istringstream& operator>>(std::istringstream &is, Vector3G<T>& v)
{
    char _;
    is >> _ >> v.x >> _ >> v.y >> _ >> v.z >> _;
    return is;
}

template<class T>
std::istringstream& operator>>(std::istringstream &is, Vector4G<T>& v)
{
    char _;
    is >> _ >> v.x >> _ >> v.y >> _ >> v.z >> _ >> v.w >> _;
    return is;
}

template<class EnumClass, class=TT_ENUM(EnumClass)>
std::istringstream& operator>>(std::istringstream &is, EnumClass& e)
{
    int x;
    is >> x;
    e = SCAST<EnumClass>(x);
    return is;
}


#endif // STREAMOPERATORS_H
