#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

class String : public std::string
{
public:
    String();
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
};

#endif // STRING_H
