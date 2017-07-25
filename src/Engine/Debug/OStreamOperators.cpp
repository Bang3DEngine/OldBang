#include "Bang/OStreamOperators.h"

#include "Bang/Color.h"
#include "Bang/IToString.h"

std::ostream& operator<<(std::ostream &log, const Color &c)
{
    log << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return log;
}

std::ostream& operator<<(std::ostream &log, const String &str)
{
    log << str.ToCString();
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
