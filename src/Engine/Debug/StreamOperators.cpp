#include "Bang/StreamOperators.h"

#include "Bang/GUID.h"
#include "Bang/Path.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

NAMESPACE_BANG_BEGIN

std::ostream& operator<<(std::ostream &log, const Color &c)
{
    log << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return log;
}

std::ostream &operator<<(std::ostream &log, const IToString *s)
{
    if (!s)
    {
        log << "Null";
    }
    else
    {
        log << s->ToString();
    }
    return log;
}
//

std::ostream &operator<<(std::ostream &log, const IToString &v)
{
    log << (&v);
    return log;
}

std::istream &operator>>(std::istream &is, Color &c)
{
    char _;
    is >> _ >> c.r >> _ >> c.g >> _ >> c.b >> _ >> c.a >> _;
    return is;
}

std::istream &operator>>(std::istream &is, Path &p)
{
    String str;
    is >> str;
    p.SetPath(str);
    return is;
}

std::istream &operator>>(std::istream &is, GUID &guid)
{
    guid.operator>>(is);
    return is;
}

NAMESPACE_BANG_END
