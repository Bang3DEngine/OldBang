#include "Bang/OStreamOperators.h"

// ostream operators
OPERATOR_OSTREAM_DEF(Path)
OPERATOR_OSTREAM_DEF(Color)
OPERATOR_OSTREAM_DEF(Vector2)
OPERATOR_OSTREAM_DEF(Vector3)
OPERATOR_OSTREAM_DEF(Vector4)
OPERATOR_OSTREAM_DEF(Matrix4)
OPERATOR_OSTREAM_DEF(IToString)
OPERATOR_OSTREAM_DEF(Quaternion)

std::ostream& operator<<(std::ostream &log, const String &str)
{
    log << str.ToCString(); return log;
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
    log << str.toStdString(); return log;
}
