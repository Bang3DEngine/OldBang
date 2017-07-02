#ifndef OSTREAMOPERATORS_H
#define OSTREAMOPERATORS_H

#include "Bang/Set.h"
#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix4.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"

#define OPERATOR_OSTREAM_DECL(CLASS) \
std::ostream& operator<<(std::ostream &log, const CLASS &v);
#define OPERATOR_OSTREAM_DEF(CLASS) \
std::ostream& operator<<(std::ostream &log, const CLASS &v) { \
    log << v.ToString(); return log; \
}
OPERATOR_OSTREAM_DECL(Path)
OPERATOR_OSTREAM_DECL(Color)
OPERATOR_OSTREAM_DECL(Vector2)
OPERATOR_OSTREAM_DECL(Vector3)
OPERATOR_OSTREAM_DECL(Vector4)
OPERATOR_OSTREAM_DECL(Matrix4)
OPERATOR_OSTREAM_DECL(IToString)
OPERATOR_OSTREAM_DECL(Quaternion)
std::ostream& operator<<(std::ostream &log, const String &str);
std::ostream& operator<<(std::ostream &log, const QString &str);
std::ostream& operator<<(std::ostream &log, const IToString *s);

// Templated ostream operators
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
