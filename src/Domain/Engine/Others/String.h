#ifndef STRING_H
#define STRING_H

#include <string>
#include <sstream>
#include <cstring>

#include "Array.h"

class Color;
class QString;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class Quaternion;
class String : public std::string
{
public:
    typedef typename std::string::iterator Iterator;
    typedef typename std::string::const_iterator Const_Iterator;
    typedef typename std::string::reverse_iterator Reverse_Iterator;
    typedef typename std::string::const_reverse_iterator Const_Reverse_Iterator;

    String();
    String(float v);
    String(const char *cstr);
    String(const std::string &stdstr);
    String(const Array<char>::Iterator begin,
           const Array<char>::Iterator end);
    String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
           std::istreambuf_iterator<char, std::char_traits<char> > end);

    char At(int index) const;

    String TrimmedLeft();
    String TrimmedRight();
    String Trimmed();

    Array<String> Split(char splitter, bool trimResults = false) const;

    void Insert(Iterator it, char c);

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end(); }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    Reverse_Iterator RBegin() { return this->rbegin(); }
    Reverse_Iterator REnd() { return this->rend(); }
    Const_Reverse_Iterator RBegin() const { return this->rbegin(); }
    Const_Reverse_Iterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::string::begin(); }
    Iterator end() { return this->std::string::end(); }
    Const_Iterator begin() const { return this->std::string::begin(); }
    Const_Iterator end() const { return this->std::string::end(); }

    long IndexOf(char c, long startingPos = 0) const;
    long IndexOf(const String &str, long startingPos = 0) const;
    long IndexOfOneOf(const String &str, long startingPos = 0) const;
    long IndexOfOneNotOf(const String &str, long startingPos = 0) const;

    // Both startIndex and endIndex are inclusive
    String SubString(long startIndex,
                     long endIndex = std::string::npos) const;

    const char *ToCString() const;
    QString ToQString() const;

    long Length() const;
    bool Empty() const;
    bool Contains(const String& str) const;

    String& operator=(const char *cstr);

    static String ToString(int i);
    static String ToString(float f);
    static String ToString(void *v);
    static String ToString(const Color &v);
    static String ToString(const Vector2 &v);
    static String ToString(const Vector3 &v);
    static String ToString(const Vector4 &v);
    static String ToString(const Matrix4 &v);
    static String ToString(const Quaternion &v);
};

// OPERATORS
// Can't use templates because it goes crazy if I try
// to use them partially/specializing

// With String
#define OPERATOR_PLUS_RIGHT_DECL(CLASS) \
String operator+(const String &str, const CLASS &v);
#define OPERATOR_PLUS_RIGHT_DEF(CLASS) \
String operator+(const String &str, const CLASS &v) { \
    String res = str; res += String::ToString(v); return res; }

#define OPERATOR_PLUS_LEFT_DECL(CLASS) \
String operator+(const CLASS &v, const String &str);
#define OPERATOR_PLUS_LEFT_DEF(CLASS) \
String operator+(const CLASS &v, const String &str) { \
    String res = str; res += String::ToString(v); return res; }


OPERATOR_PLUS_RIGHT_DECL(Color)
 OPERATOR_PLUS_LEFT_DECL(Color)
OPERATOR_PLUS_RIGHT_DECL(Vector2)
 OPERATOR_PLUS_LEFT_DECL(Vector2)
OPERATOR_PLUS_RIGHT_DECL(Vector3)
 OPERATOR_PLUS_LEFT_DECL(Vector3)
OPERATOR_PLUS_RIGHT_DECL(Vector4)
 OPERATOR_PLUS_LEFT_DECL(Vector4)
OPERATOR_PLUS_RIGHT_DECL(Matrix4)
 OPERATOR_PLUS_LEFT_DECL(Matrix4)
OPERATOR_PLUS_RIGHT_DECL(Quaternion)
 OPERATOR_PLUS_LEFT_DECL(Quaternion)
/*
OPERATOR_PLUS_RIGHT_PRIMITIVE_DECL(int)
 OPERATOR_PLUS_LEFT_PRIMITIVE_DECL(int)
OPERATOR_PLUS_RIGHT_PRIMITIVE_DECL(float)
 OPERATOR_PLUS_LEFT_PRIMITIVE_DECL(float)
 */

// These two below let us write for example:
//    String str = "wololo" + Vector2(13,37);
//                    ^ (explicitely casted to String,
//                       and now can use all the operator+ methods declared above)

template <class T>
String operator+(const char *str, const T &v)
{
    return String(str) + v;
}
template <class T>
String operator+(const T &v, const char *str)
{
    return v + String(str);
}

#endif // STRING_H
