#include "String.h"

#include <QString>

#include "Map.h"
#include "List.h"
#include "Array.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

String::String() :
    std::string("")
{
}

String::String(float v)  :
    std::string("")
{
    *this = ToString(v);
}

String::String(const char *cstr) :
    std::string(cstr)
{
}

String::String(const QString &qstr)
{
    *this = qstr.toStdString();
}

String::String(const std::string &stdstr) :
    std::string(stdstr)
{
}

String::String(const std::vector<char>::iterator begin,
               const std::vector<char>::iterator end) :
    std::string(begin, end)
{
}

String::String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
               std::istreambuf_iterator<char, std::char_traits<char> > end) :
    std::string(begin, end)
{
}

char String::At(int index) const
{
    return at(index);
}

String String::TrimmedLeft()
{
    String result = *this;
    if (!Empty())
    {
        int i = 0;
        for (; i < Length(); ++i)
        {
            if (At(i) != ' ' && At(i) != '\t') break;
        }

        if (i == Length()) { result = ""; }
        else { result = result.SubString(i); }
    }
    return result;
}

String String::TrimmedRight()
{
    String result = *this;
    if (!Empty())
    {
        int i = Length() - 1;
        for (; i >= 0; --i)
        {
            if (At(i) != ' ' && At(i) != '\t') break;
        }

        if (i < 0) { result = ""; }
        else { result = result.SubString(0, i); }
    }
    return result;
}

String String::Trimmed()
{
    return (*this).TrimmedLeft().TrimmedRight();
}

Array<String> String::Split(char splitter, bool trimResults) const
{
    Array<String> result;
    if (Empty()) { return result; }

    bool lastParticle = false;
    long lastIndexFound = 0;
    while (!lastParticle)
    {
        long indexFound = IndexOf(splitter, lastIndexFound);
        if (indexFound == -1)
        {
            lastParticle = true;
            indexFound = Length();
        }

        String particle = SubString(lastIndexFound, indexFound - 1);
        if (trimResults)
        {
            particle = particle.Trimmed();
        }
        result.PushBack(particle);

        lastIndexFound = indexFound + 1;
    }
    return result;
}

String String::Join(const Array<String> &parts, String joiner)
{
    String all = "";
    for (int i = 0; i < parts.Size(); ++i)
    {
        const String &part = parts[i];
        all += part;
        if (i < parts.Size() - 1)
        {
            all += joiner;
        }
    }
    return all;
}

String String::Join(const List<String> &parts, String joiner)
{
    int i = 0;
    String all = "";
    for (auto it = parts.Begin(); it != parts.End(); ++it)
    {
        const String &part = *it;
        all += part;
        if (i < parts.Size() - 1)
        {
            all += joiner;
        }
        ++i;
    }
    return all;
}

void String::Append(const String &str)
{
    *this = *this + str;
}

void String::Prepend(const String &str)
{
    *this = str + *this;
}

void String::Insert(Iterator it, char c)
{
    insert(it, c);
}

long String::IndexOf(char c, long startingPos) const
{
    return find(c, startingPos);
}

long String::IndexOf(const String &str, long startingPos) const
{
    return find(str, startingPos);
}

long String::IndexOfOneOf(const String &str, long startingPos) const
{
    return find_first_of(str, startingPos);
}

long String::IndexOfOneNotOf(const String &str, long startingPos) const
{
    return find_first_not_of(str, startingPos);
}

String String::SubString(long startIndex, long endIndex) const
{
    if (endIndex == std::string::npos) return substr(startIndex);
    else return substr(startIndex, endIndex - startIndex + 1);
}

const char *String::ToCString() const
{
    return c_str();
}

QString String::ToQString() const
{
    return QString::fromStdString(*this);
}

long String::Length() const
{
    return length();
}

bool String::Empty() const
{
    return Length() == 0;
}

bool String::Contains(const String &str) const
{
    return find(str) != std::string::npos;
}

bool String::BeginsWith(const String &str) const
{
    return this->IndexOf(str) == 0;
}

bool String::EndsWith(const String &str) const
{
    return this->IndexOf(str) == Length() - str.Length();
}


String& String::operator=(const char *cstr)
{
    std::string::operator=(cstr);
    return *this;
}

String String::ToString(int i)
{
    return String(std::to_string(i));
}

String String::ToString(float f)
{
    return String(std::to_string(f));
}

String String::ToString(void *v)
{
    std::ostringstream log;
    log << v;
    return String(log.str());
}

String String::ToString(const Color &v)
{
    return v.ToString();
}
String String::ToString(const Vector2 &v)
{
    return v.ToString();
}
String String::ToString(const Vector3 &v)
{
    return v.ToString();
}
String String::ToString(const Vector4 &v)
{
    return v.ToString();
}
String String::ToString(const Matrix4 &v)
{
    return v.ToString();
}
String String::ToString(const Quaternion &v)
{
    return v.ToString();
}

// OPERATORS

// Operator+ using String::ToString(v)
// (so String::ToString(const T &v) must be explicitely defined for T
//  If not, this will give compile errors.)
OPERATOR_PLUS_RIGHT_DEF(Color)
 OPERATOR_PLUS_LEFT_DEF(Color)
OPERATOR_PLUS_RIGHT_DEF(Vector2)
 OPERATOR_PLUS_LEFT_DEF(Vector2)
OPERATOR_PLUS_RIGHT_DEF(Vector3)
 OPERATOR_PLUS_LEFT_DEF(Vector3)
OPERATOR_PLUS_RIGHT_DEF(Vector4)
 OPERATOR_PLUS_LEFT_DEF(Vector4)
OPERATOR_PLUS_RIGHT_DEF(Matrix4)
 OPERATOR_PLUS_LEFT_DEF(Matrix4)
OPERATOR_PLUS_RIGHT_DEF(Quaternion)
 OPERATOR_PLUS_LEFT_DEF(Quaternion)
/*
OPERATOR_PLUS_RIGHT_PRIMITIVE_DEF(int)
 OPERATOR_PLUS_LEFT_PRIMITIVE_DEF(int)
OPERATOR_PLUS_RIGHT_PRIMITIVE_DEF(float)
 OPERATOR_PLUS_LEFT_PRIMITIVE_DEF(float)
 */
//
