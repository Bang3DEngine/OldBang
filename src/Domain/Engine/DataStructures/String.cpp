#include "Bang/String.h"

#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <QString>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/Debug.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix4.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"

String::String() : std::string("")
{
}

String::String(int v) : std::string("")
{
    *this = ToString(v);
}

String::String(float v)  : std::string("")
{
    *this = ToString(v);
}

String::String(const char *cstr) : std::string(cstr)
{
}

String::String(const QString &qstr)
{
    *this = String(qstr.toStdString());
}

// .c_str() guarrantees no CopyOnWrite so it's thread-safe (?)
String::String(const std::string &stdstr) : std::string(stdstr.c_str())
{
}

String::String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
               std::istreambuf_iterator<char, std::char_traits<char> > end) :
    std::string(begin, end)
{
}

String::~String()
{
}

char String::At(int index) const
{
    return at(index);
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

        if (indexFound == lastIndexFound)
        {
            result.PushBack("");
        }
        else
        {
            String particle = SubString(lastIndexFound, indexFound - 1);
            if (trimResults)
            {
                particle = particle.Trim();
            }
            result.PushBack(particle);
        }
        lastIndexFound = indexFound + 1;
    }
    return result;
}

String String::Join(const Array<String> &parts, String joiner)
{
    return String::Join(parts.To<List>(), joiner);
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

String::Iterator String::Insert(Iterator it, char c)
{
    return insert(it, c);
}

String::Iterator String::Insert(String::Iterator it, const String &str)
{
    return insert(it, str.Begin(), str.End());
}

void String::Insert(int position, char c)
{
    Iterator pos = begin();
    std::advance(pos, position);
    insert(pos, c);
}

void String::Insert(int position, const String &str)
{
    insert(position, str.ToCString());
}

void String::Erase(String::Iterator it, int numberOfChars)
{
    Iterator end = it;
    std::advance(end, numberOfChars);
    erase(it, end);
}

void String::Erase(int beginIndex, int endIndexInclusive)
{
    Iterator begin = Begin();
    std::advance(begin, beginIndex);
    Iterator end = begin;
    std::advance(end, endIndexInclusive - beginIndex + 1);
    erase(begin, end);
}

long String::IndexOf(char c, long startingPos) const
{
    int index = find(c, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOf(const String &str, long startingPos) const
{
    int index = find(str, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOfOneOf(const String &charSet, long startingPos) const
{
    int index = find_first_of(charSet, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOfOneNotOf(const String &charSet, long startingPos) const
{
    int index = find_first_not_of(charSet, startingPos);
    return index == String::npos ? - 1 : index;
}

String String::SubString(long startIndexInclusive, long endIndexInclusive) const
{
    if (endIndexInclusive == String::npos)
    {
        return substr(startIndexInclusive);
    }
    return substr(startIndexInclusive,
                  endIndexInclusive - startIndexInclusive + 1);
}

const char *String::ToCString() const
{
    return c_str();
}

QString String::ToQString() const
{
    return QString::fromStdString(*this);
}

int String::ReplaceInSitu(const String &from,
                    const String &to,
                    int maxNumberOfReplacements)
{
	if (from.Empty()) { return 0; }

    int lastIndex = 0;
    int numReplacements = 0;
    while (lastIndex != -1 && lastIndex + from.Length() <= Length())
    {
        lastIndex = IndexOf(from, lastIndex);
        if (lastIndex >= 0)
        {
            Erase(lastIndex, lastIndex + from.Length() - 1);
            Insert(lastIndex, to);
            lastIndex += to.Length();
            ++numReplacements;
            if (maxNumberOfReplacements != -1 &&
                numReplacements >= maxNumberOfReplacements)
            {
                break;
            }
        }
    }
    return numReplacements;
}

String String::Replace(const String &from, const String &to,
                       int maxNumberOfReplacements) const
{
    String str = *this;
    str.ReplaceInSitu(from, to, maxNumberOfReplacements);
    return str;
}

String String::Elide(int length, bool elideRight) const
{
    int maxLength = std::min(int(Length()), length);
    String result = (*this);
    if (result.Length() > length)
    {
        result = result.SubString(result.Length() - maxLength,
                                  result.Length() - 1);
        if (elideRight) { result = result + "..."; }
        else            { result = "..." + result; }
    }
    return result;
}

String String::ElideRight(int length) const
{
    return Elide(length, true);
}

String String::ElideLeft(int length) const
{
    return Elide(length, false);
}

String String::TrimLeft(List<char> trimChars) const
{
    if(Empty()) { return ""; }

    int i = 0;
    for (; i < Length(); ++i)
    {
        if (!trimChars.Contains( At(i) )) break;
    }
    return (i == Length()) ? "" : SubString(i, Length());
}

String String::TrimRight(List<char> trimChars) const
{
    if (Empty()) { return ""; }

    int i = Length() - 1;
    for (; i >= 0; --i)
    {
        if (!trimChars.Contains( At(i) )) break;
    }
    return (i < 0) ? "" : SubString(0, i);
}

String String::Trim(List<char> trimChars) const
{
    return (*this).TrimLeft(trimChars).TrimRight(trimChars);
}

String String::TrimLeft() const
{
    return TrimLeft({' ', '\t'});
}

String String::TrimRight() const
{
    return TrimRight({' ', '\t'});
}

String String::Trim() const
{
    return Trim({' ', '\t'});
}

String String::AddInFrontOfWords(String particle) const
{
    String result = *this;
    if (!result.Empty() && result.At(0) != ' ') { result.Insert(0, particle); }

    for (int i = 0; i < result.Length() - 1; ++i)
    {
        if (result.At(i) == ' ' && result.At(i+1) != ' ')
        {
            result.Insert(i+1, particle);
            i += 2;
        }
    }
    return result;
}


bool String::IsNumber(char c)
{
    return c >= 48 && c <= 57;
}

bool String::IsLetter(char c)
{
    return  IsLowerCase(c) || IsUpperCase(c);
}

bool String::IsUpperCase(char c)
{
    return c >= 65 && c <= 90;
}

bool String::IsLowerCase(char c)
{
    return c >= 97 && c <= 122;
}

char String::ToUpper(char c)
{
    return toupper(c);
}

char String::ToLower(char c)
{
    return tolower(c);
}

int String::ToInt(const String &str, bool *ok)
{
    String number = str.Trim();
    std::istringstream iss(number);
    int v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

float String::ToFloat(const String &str, bool *ok)
{
    String number = str.Trim();
    std::istringstream iss(number);
    float v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

long String::Length() const
{
    return length();
}

bool String::EqualsNoCase(const String &str) const
{
    return Contains(str, false) && (Length() == str.Length());
}

bool String::Empty() const
{
    return Length() == 0;
}

bool String::Contains(const String &str, bool caseSensitive) const
{
    if (!caseSensitive)
    {
        auto it = std::search(Begin(), End(), str.Begin(), str.End(),
          [](char ch1, char ch2) {
            return String::ToUpper(ch1) == String::ToUpper(ch2);
          }
        );
        return (it != End() );
    }
    return find(str) != String::npos;
}

bool String::BeginsWith(const String &str) const
{
    return this->IndexOf(str) == 0;
}

bool String::EndsWith(const String &str) const
{
    return this->IndexOf(str) == Length() - str.Length();
}

String String::ToUpper() const
{
    String result = *this;
    for (int i = 0; i < Length(); ++i)
    {
        result[i] = String::ToUpper(result[i]);
    }
    return result;
}

String String::ToLower() const
{
    String result = *this;
    for (int i = 0; i < Length(); ++i)
    {
        result[i] = String::ToLower(result[i]);
    }
    return result;
}

std::size_t String::operator()(const String &str) const
{
    return std::hash<std::string>()(str);
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
String String::ToString(long v)
{
    return String(std::to_string(v));
}

String String::ToString(bool v)
{
    return v ? "True" : "False";
}
String String::ToString(long long unsigned v)
{
    return String(std::to_string(v));
}

String String::ToString(float f, int decimalPlaces)
{
    return ToString(static_cast<double>(f), decimalPlaces);
}

String String::ToString(double f, int decimalPlaces)
{
    String str = "";
    if (decimalPlaces >= 0)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(decimalPlaces) << f;
        str = oss.str();
    }
    else
    {
        str = String(std::to_string(f));
    }
    return str;
}

String String::ToString(const void *v)
{
    std::ostringstream log;
    log << v;
    return String(log.str());
}

String String::ToString(const Color &v)
{
    return String(v.ToString());
}

String String::ToString(const String &v)
{
    // .ToCString() to avoid COW so it's thread safe
    return String(v.ToCString());
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

String String::ToString(const IToString &v)
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
