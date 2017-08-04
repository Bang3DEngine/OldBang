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

class String
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
    String(Iterator begin, Iterator end) : m_str(begin, end) { }

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

    std::size_t Find(char c, std::size_t toIndex  = String::npos) const;
    std::size_t RFind(char c, std::size_t toIndex = String::npos) const;
    std::size_t
    Find(const char *str, std::size_t fromIndex, std::size_t length) const;
    std::size_t
    RFind(const char *str, std::size_t fromIndex, std::size_t length) const;

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

    friend std::istream& operator>>(std::istream &is, String &str);
    friend std::ostream& operator<<(std::ostream &os, const String &str);
    friend String operator+(const String &str1, const String &str2);
    friend bool operator<(const String &str1, const String &str2);
    friend bool operator<=(const String &str1, const String &str2);
    friend bool operator>(const String &str1, const String &str2);
    friend bool operator>=(const String &str1, const String &str2);
    friend bool operator==(const String &str1, const String &str2);
    friend bool operator!=(const String &str1, const String &str2);
    friend String& operator+=(String &str1, const String &str2);
    char operator[](std::size_t i) const;
    char& operator[](std::size_t i);
    operator std::string() const;
    std::size_t operator()(const String &str) const;
    String& operator=(const char *cstr);

    Iterator Begin() { return m_str.begin(); }
    Iterator End() { return m_str.end(); }
    Const_Iterator Begin() const { return m_str.begin(); }
    Const_Iterator End() const { return m_str.end(); }
    RIterator RBegin() { return m_str.rbegin(); }
    RIterator REnd() { return m_str.rend(); }
    Const_RIterator RBegin() const { return m_str.rbegin(); }
    Const_RIterator REnd() const { return m_str.rend(); }

    // To allow range-based for loops
    Iterator begin() { return m_str.begin(); }
    Iterator end() { return m_str.end(); }
    Const_Iterator begin() const { return m_str.begin(); }
    Const_Iterator end() const { return m_str.end(); }

    static constexpr std::size_t npos = std::string::npos;

private:
    std::string m_str;
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

String operator+(const String &str1, const String &str2);

template <class T>
String operator+(const char *str, const T &v)
{
    return String(std::string(str) + std::string(String::ToString(v)));
}
template <class T>
String operator+(const T &v, const char *str)
{
    return String(std::string(String::ToString(v)) + std::string(str));
}  

#endif // STRING_H
