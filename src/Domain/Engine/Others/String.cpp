#include "String.h"

String::String()
{
}

String::String(const char *cstr) :
    std::string(cstr)
{
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

std::vector<String> String::Split(char splitter, bool trimResults) const
{
    std::vector<String> result;
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
        result.push_back(particle);

        lastIndexFound = indexFound + 1;
    }
    return result;
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


String& String::operator=(const char *cstr)
{
    std::string::operator=(cstr);
    return *this;
}
