#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

class Color;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class Quaternion;
class String : public std::string
{
public:
    String();
    String(float v);
    String(const char *cstr);
    String(const std::string &stdstr);
    String(const std::vector<char>::iterator begin,
           const std::vector<char>::iterator end);
    String(std::istreambuf_iterator<char, std::char_traits<char> > begin,
           std::istreambuf_iterator<char, std::char_traits<char> > end);

    char At(int index) const;

    String TrimmedLeft();
    String TrimmedRight();
    String Trimmed();

    std::vector<String> Split(char splitter, bool trimResults = false) const;

    long IndexOf(char c, long startingPos = 0) const;
    long IndexOf(const String &str, long startingPos = 0) const;
    long IndexOfOneOf(const String &str, long startingPos = 0) const;
    long IndexOfOneNotOf(const String &str, long startingPos = 0) const;

    // Both startIndex and endIndex are inclusive
    String SubString(long startIndex,
                     long endIndex = std::string::npos) const;

    const char *ToCString() const;

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
/*
// With const char*
#define OPERATOR_PLUS_RIGHT_CONST_CHAR_DECL(CLASS) \
String operator+(const char *str, const CLASS &v);
#define OPERATOR_PLUS_RIGHT_CONST_CHAR_DEF(CLASS) \
String operator+(const char *str, const CLASS &v) { \
    String res = String(str); res += String::ToString(v); return res; }

#define OPERATOR_PLUS_LEFT_CONST_CHAR_DECL(CLASS) \
String operator+(const CLASS &v, const char *str);
#define OPERATOR_PLUS_LEFT_CONST_CHAR_DEF(CLASS) \
String operator+(const CLASS &v, const char *str) { \
    String res = String(str); res += String::ToString(v); return res; }
*/

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
