#include "StringUtils.h"

const std::string StringUtils::TOKEN_SPACE = " \t\n";

bool StringUtils::Contains(const std::string &str, const std::string &whatToFind)
{
    return str.find(whatToFind.c_str(), 0) != std::string::npos;
}

float StringUtils::ToFloat(const std::string &str, bool *ok)
{
    std::string number = str;
    StringUtils::Trim(&number);
    std::istringstream iss(number);
    float v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

int StringUtils::ToInt(const std::string &str, bool *ok)
{
    std::string number = str;
    StringUtils::Trim(&number);
    std::istringstream iss(number);
    int
            v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

std::string StringUtils::FromFloat(float f, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << f;
    return oss.str();
}

void StringUtils::TrimLeft(std::string *str)
{
    if(str->length() == 0)
    {
        return;
    }

    int i = 0;
    for (; i < str->length(); ++i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = i == str->length() ? "" : str->substr(i, str->length() - i);
}

void StringUtils::TrimRight(std::string *str)
{
    if(str->length() == 0)
    {
        return;
    }

    int i = str->length()-1;
    for (; i >= 0; --i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = i < 0 ? "" : str->substr(0, i+1);
}

void StringUtils::Trim(std::string *str)
{
    StringUtils::TrimLeft(str);
    StringUtils::TrimRight(str);
}

std::string StringUtils::Trim(const std::string &str)
{
    std::string strCpy = str;
    Trim(&strCpy);
    return str;
}

std::string StringUtils::Concat(const std::vector<std::string> &particles, std::string joiner)
{
    std::string result = "";
    for (int i = 0; i < particles.size(); ++i)
    {
        if (i != 0) { result += joiner; }
        result += particles[i];
    }
    return result;
}


std::vector<std::string> StringUtils::Split(const std::string &content, char splitter)
{
    std::vector<std::string> result;
    if(content == "")
    {
        return result; // Empty
    }

    bool lastParticle = false;
    int lastIndexFound = 0;
    while (!lastParticle)
    {
        int indexFound = content.find_first_of(splitter, lastIndexFound);
        if (indexFound == std::string::npos)
        {
            lastParticle = true;
            indexFound = content.length();
        }
        std::string particle = content.substr(lastIndexFound, indexFound - lastIndexFound);
        result.push_back(particle);
        lastIndexFound = indexFound + 1;
    }
    return result;
}

std::vector<std::string> StringUtils::SplitTrim(const std::string &content, char splitter)
{
    std::vector<std::string> result = Split(content, splitter);
    for (std::string &str : result)
    {
        Trim(&str);
    }
    return result;
}

std::vector<std::string> StringUtils::BangEnumVariadicStringToNamesArray(const std::string &_va_args_)
{
    // We receive something like "Wololo, Apple = 49, Pear=29, Lololo=2193, Banana,Sandwich, Monkey=32"
    // We want this vector: ["Wololo", "Apple", "Pear", "Lololo", "Banana", "Sandwich", "Monkey"]
    std::vector<std::string> result = SplitTrim(_va_args_, ',');
    for (std::string &str : result)
    {
        str = SplitTrim(str, '=')[0];
    }
    return result;
}

std::string StringUtils::FormatInspectorLabel(const std::string &labelString)
{
    std::string labelFormatted = labelString;
    StringUtils::Replace(&labelFormatted, "_", " ");

    // Add space after every cap (except for consecutive caps).
    for (auto it = labelFormatted.begin(); it != labelFormatted.end(); ++it)
    {
        ++it;
        bool nextOneIsCap = (it != labelFormatted.end()) && IsCap(*it);
        --it;
        if (!nextOneIsCap && IsCap(*it) && it != labelFormatted.begin())
        {
            it = labelFormatted.insert(it, ' ');
            ++it;
        }
    }

    StringUtils::Replace(&labelFormatted, "  ", " ");

    return labelFormatted;
}

bool StringUtils::IsNumber(char c)
{
    return c >= 48 && c <= 57;
}

bool StringUtils::IsLetter(char c)
{
    return (c >= 97 && c <= 122) || IsCap(c);
}

bool StringUtils::IsCap(char c)
{
    return c >= 65 && c <= 90;
}

void StringUtils::AddInFrontOfWords(std::string particle, std::string *str)
{
    std::string &phrase = *str;
    if (phrase.length() > 0 && phrase[0] != ' ')
    {
        phrase.insert(0, particle);
    }

    for (int i = 0; i < phrase.length() -1; ++i)
    {
        if (phrase[i] == ' ' && phrase[i+1] != ' ')
        {
            phrase.insert(i+1, particle);
            i += 2; // Sorry
        }
    }
}

void StringUtils::RemoveLineBreaks(std::string *str)
{
    std::replace(str->begin(), str->end(), '\n', ' ');
}

void StringUtils::Replace(std::string *content, const std::string &toFind, const std::string &replaceWithThis)
{
    std::string::size_type i;
    while ( (i = content->find(toFind)) != std::string::npos)
    {
        content->replace(i, toFind.length(), replaceWithThis);
    }
}
