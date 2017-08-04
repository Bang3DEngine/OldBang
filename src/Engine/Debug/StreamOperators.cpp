#include "Bang/StreamOperators.h"

#include <QString>

#include "Bang/Path.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

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


std::ostream &operator<<(std::ostream &log, const QString &str)
{
    log << str.toStdString();
    return log;
}

std::ostream &operator<<(std::ostream &log, const IToString &v)
{
    log << (&v);
    return log;
}

std::istringstream &operator>>(std::istringstream &is, Color &c)
{
    char _;
    is >> _ >> c.r >> _ >> c.g >> _ >> c.b >> _ >> c.a >> _;
    return is;
}

std::istringstream &operator>>(std::istringstream &is, Path &p)
{
    String str;
    is >> str;
    p.SetPath(str);
    return is;
}
