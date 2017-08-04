#include "Bang/String.h"

#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <QString>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/IToString.h"

String::String() : m_str("")
{
}

String::String(int v) : m_str("")
{
    *this = ToString(v);
}

String::String(float v)  : m_str("")
{
    *this = ToString(v);
}

String::String(const char *cstr) : m_str(cstr)
{
}

String::String(const QString &qstr)
{
    *this = String(qstr.toStdString());
}

String::String(const std::string &stdstr) : m_str(stdstr)
{
}

String::String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
               std::istreambuf_iterator<char, std::char_traits<char> > end) :
    m_str(begin, end)
{
}

String::~String()
{
}

char String::At(int index) const
{
    return m_str.at(index);
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
            indexFound = Size();
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
    return m_str.insert(it, c);
}

void String::Insert(int position, char c)
{
    Iterator pos = begin();
    std::advance(pos, position);
    m_str.insert(pos, c);
}

void String::Insert(int position, const String &str)
{
    m_str.insert(position, str.ToCString());
}

void String::Erase(String::Iterator it, int numberOfChars)
{
    Iterator end = it;
    std::advance(end, numberOfChars);
    m_str.erase(it, end);
}

void String::Erase(int beginIndex, int endIndexInclusive)
{
    Iterator begin = Begin();
    std::advance(begin, beginIndex);
    Iterator end = begin;
    std::advance(end, endIndexInclusive - beginIndex + 1);
    m_str.erase(begin, end);
}

long String::IndexOf(char c, long startingPos) const
{
    int index = m_str.find(c, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOf(const String &str, long startingPos) const
{
    int index = m_str.find(str, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOfOneOf(const String &charSet, long startingPos) const
{
    int index = m_str.find_first_of(charSet, startingPos);
    return index == String::npos ? - 1 : index;
}

long String::IndexOfOneNotOf(const String &charSet, long startingPos) const
{
    int index = m_str.find_first_not_of(charSet, startingPos);
    return index == String::npos ? - 1 : index;
}

String String::SubString(long startIndexInclusive, long endIndexInclusive) const
{
    if (startIndexInclusive >= Size()) { return ""; }
    if (endIndexInclusive   >= Size()) { endIndexInclusive = Size()-1; }

    if (endIndexInclusive == String::npos)
    {
        return m_str.substr(startIndexInclusive);
    }
    return m_str.substr(startIndexInclusive,
                        endIndexInclusive - startIndexInclusive + 1);
}

const char *String::ToCString() const
{
    return m_str.c_str();
}

QString String::ToQString() const
{
    return QString::fromStdString(*this);
}

std::size_t String::Find(char c, std::size_t toIndex) const
{
    return m_str.find(c, toIndex);
}
std::size_t String::RFind(char c, std::size_t toIndex) const
{
    return m_str.rfind(c, toIndex);
}
std::size_t String::Find(const char *str,
                         std::size_t fromIndex,
                         std::size_t length) const
{
    return m_str.find(str, fromIndex, length);
}
std::size_t String::RFind(const char *str,
                          std::size_t fromIndex,
                          std::size_t length) const
{
    return m_str.rfind(str, fromIndex, length);
}

int String::ReplaceInSitu(const String &from,
                          const String &to,
                          int maxNumberOfReplacements)
{
    if (from.Empty()) { return 0; }

    int lastIndex = 0;
    int numReplacements = 0;
    while (lastIndex != -1 && lastIndex + from.Size() <= Size())
    {
        lastIndex = IndexOf(from, lastIndex);
        if (lastIndex >= 0)
        {
            Erase(lastIndex, lastIndex + from.Size() - 1);
            Insert(lastIndex, to);
            lastIndex += to.Size();
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
    int maxLength = std::min(int(Size()), length);
    String result = (*this);
    if (result.Size() > length)
    {
        result = result.SubString(result.Size() - maxLength,
                                  result.Size() - 1);
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
    for (; i < Size(); ++i)
    {
        if (!trimChars.Contains( At(i) )) break;
    }
    return (i == Size()) ? "" : SubString(i, Size());
}

String String::TrimRight(List<char> trimChars) const
{
    if (Empty()) { return ""; }

    int i = Size() - 1;
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

    for (int i = 0; i < result.Size() - 1; ++i)
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

long String::Size() const
{
    return m_str.length();
}

bool String::EqualsNoCase(const String &str) const
{
    return Contains(str, false) && (Size() == str.Size());
}

bool String::Empty() const
{
    return Size() == 0;
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
    return m_str.find(str) != String::npos;
}

bool String::BeginsWith(const String &str) const
{
    return this->IndexOf(str) == 0;
}

bool String::EndsWith(const String &str) const
{
    return this->IndexOf(str) == Size() - str.Size();
}

String String::ToUpper() const
{
    String result = *this;
    for (int i = 0; i < Size(); ++i)
    {
        result[i] = String::ToUpper(result[i]);
    }
    return result;
}

String String::ToLower() const
{
    String result = *this;
    for (int i = 0; i < Size(); ++i)
    {
        result[i] = String::ToLower(result[i]);
    }
    return result;
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

String String::ToString(const String &v)
{
    return v;
}

String String::ToString(const IToString &v)
{
    return v.ToString();
}

std::ostream& operator<<(std::ostream &os, const String &str)
{
    return os << str.m_str;
}

char& String::operator[](std::size_t i)
{
    return m_str[i];
}

char String::operator[](std::size_t i) const
{
    return m_str[i];
}

std::istream& operator>>(std::istream &is, String &str)
{
    is >> str.m_str;
    return is;
}

String& operator+=(String &str1, const String &str2)
{
    str1.m_str += str2.m_str;
    return str1;
}

bool operator<(const String &str1, const String &str2)
{
    return str1.m_str < str2.m_str;
}
bool operator<=(const String &str1, const String &str2)
{
    return str1.m_str <= str2.m_str;
}
bool operator>(const String &str1, const String &str2)
{
    return str1.m_str > str2.m_str;
}
bool operator>=(const String &str1, const String &str2)
{
    return str1.m_str >= str2.m_str;
}
bool operator==(const String &str1, const String &str2)
{
    return str1.m_str == str2.m_str;
}
bool operator!=(const String &str1, const String &str2)
{
    return str1.m_str != str2.m_str;
}

String::operator std::string() const
{
    return m_str;
}

std::size_t String::operator()(const String &str) const
{
    return std::hash<std::string>()(str);
}

String& String::operator=(const char *cstr)
{
    m_str = cstr;
    return *this;
}

String operator+(const String &str1, const String &str2)
{
    return String(str1.m_str + str2.m_str);
}
