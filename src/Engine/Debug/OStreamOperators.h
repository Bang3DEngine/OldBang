#ifndef OSTREAMOPERATORS_H
#define OSTREAMOPERATORS_H

#include <sstream>

class Color;
class String;
class QString;
class IToString;
template<class T> class Set;
template<class T> class List;
template<class T> class Array;
template<class T> class RectG;
template<class T> class Vector2G;
template<class T> class Vector3G;
template<class T> class Vector4G;
template<class T> class Matrix3G;
template<class T> class Matrix4G;
template<class T> class QuaternionG;
template<class K, class V> class Map;

std::ostream& operator<<(std::ostream &log, const Color &v);
std::ostream& operator<<(std::ostream &log, const IToString &v);
std::ostream& operator<<(std::ostream &log, const String &str);
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
    log << "(" << r.GetMin().x << ", " <<
                  r.GetMin().y << ", " <<
                  r.GetMax().x << ", " <<
                  r.GetMax().y << ")";
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
    for (auto it = m->Begin(); it != m->End(); ++it)
    {
        if (it != m->begin()) log << ", ";
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
    for (auto it = s->begin(); it != s->end(); ++it)
    {
        if (it != s->begin()) log << ", ";
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

#endif // OSTREAMOPERATORS_H
