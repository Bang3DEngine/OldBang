#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

#include "Bang.h"

FORWARD   class QString;
FORWARD   class IToString;
FORWARD_T class List;
FORWARD_T class Array;

class String : public std::string
{
public:
    using Iterator = typename std::string::iterator;
    using RIterator = typename std::string::reverse_iterator;
    using Const_Iterator = typename std::string::const_iterator;
    using Const_RIterator = typename std::string::const_reverse_iterator;

    String();
    explicit String(int v);
    explicit String(float v);
    String(const char *cstr);
    String(const QString &qstr);
    String(const std::string &stdstr);
    String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
           std::istreambuf_iterator<char, std::char_traits<char> > end);

    template<class Iterator>
    String(Iterator begin, Iterator end) : std::string(begin, end) { }

    virtual ~String();

    char At(int index) const;

    Array<String> Split(char splitter, bool trimResults = false) const;

    static String Join(const Array<String> &parts, String joiner = "");
    static String Join(const List<String>  &parts, String joiner = "");

    void Append(const String &str);
    void Prepend(const String &str);

    Iterator Insert(Iterator it, char c);

    template<class TIterator>
    Iterator Insert(TIterator it, const String &str)
    {
        return insert(it, str.Begin(), str.End());
    }

    void Insert(int position, char c);
    void Insert(int position, const String &str);
    void Erase(Iterator it, int numberOfChars);
    void Erase(int beginIndex, int endIndexInclusive);

    long IndexOf(char c, long startingPos = 0) const;
    long IndexOf(const String &str, long startingPos = 0) const;
    long IndexOfOneOf(const String &charSet, long startingPos = 0) const;
    long IndexOfOneNotOf(const String &charSet, long startingPos = 0) const;

    // Both startIndex and endIndex are inclusive
    String SubString(long startIndexInclusive,
                     long endIndexInclusive = String::npos) const;

    const char *ToCString() const;
    QString ToQString() const;

    int ReplaceInSitu(const String &from, const String &to,
                int maxNumberOfReplacements = -1);
    String Replace(const String &from, const String &to,
                   int maxNumberOfReplacements = -1) const;

    String Elide(int length, bool elideRight) const;
    String ElideRight(int length) const;
    String ElideLeft(int length) const;
    String TrimLeft(List<char> trimChars) const;
    String TrimRight(List<char> trimChars) const;
    String Trim(List<char> trimChars) const;
    String TrimLeft() const;
    String TrimRight() const;
    String Trim() const;
    String AddInFrontOfWords(String particle) const;

    long Length() const;
    bool EqualsNoCase(const String &str) const;
    bool Empty() const;
    bool Contains(const String& str, bool caseSensitive = true) const;
    bool BeginsWith(const String& str) const;
    bool EndsWith(const String& str) const;
    String ToUpper() const;
    String ToLower() const;

    std::size_t operator()(const String &str) const;
    String& operator=(const char *cstr);

    static bool IsNumber(char c);
    static bool IsLetter(char c);
    static bool IsUpperCase(char c);
    static bool IsLowerCase(char c);
    static char ToUpper(char c);
    static char ToLower(char c);

    template <class T>
    static T To(const String &str, bool *ok = nullptr);

    static int ToInt(const String &str, bool *ok = nullptr);
    static float ToFloat(const String &str, bool *ok = nullptr);

    static String ToString(int i);
    static String ToString(long v);
    static String ToString(bool v);
    static String ToString(long long unsigned v);
    static String ToString(float f, int decimalPlaces = -1);
    static String ToString(double f, int decimalPlaces = -1);
    static String ToString(const void *v);
    static String ToString(const String &v);
    static String ToString(const IToString &v);

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end(); }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    RIterator RBegin() { return this->rbegin(); }
    RIterator REnd() { return this->rend(); }
    Const_RIterator RBegin() const { return this->rbegin(); }
    Const_RIterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::string::begin(); }
    Iterator end() { return this->std::string::end(); }
    Const_Iterator begin() const { return this->std::string::begin(); }
    Const_Iterator end() const { return this->std::string::end(); }
};

template <>
inline int String::To<int>(const String &str, bool *ok)
{
    return String::ToInt(str, ok);
}

template <>
inline float String::To<float>(const String &str, bool *ok)
{
    return String::ToFloat(str, ok);
}

template <class T>
String operator+(const char *str, const T &v)
{
    //return String(str) + String::ToString(v);
    return String(std::string(str) + std::string(String::ToString(v)));
}
template <class T>
String operator+(const T &v, const char *str)
{
    return String(std::string(String::ToString(v)) + std::string(str));
}  

#endif // STRING_H
