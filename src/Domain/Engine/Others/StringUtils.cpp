#include "StringUtils.h"

const std::string StringUtils::TOKEN_SPACE = " \t\n";

bool StringUtils::Contains(const std::string &str, const std::string &whatToFind)
{
    if (str.length() == 0) return false;
    if (whatToFind.length() == 0) return true;

    int i = 0;
    int j = i;
    for (; i < str.length(); ++i)
    {
        char ci = str[i];
        char cj = whatToFind[j];

        if (ci == cj)
        {
            ++j;
        }
        else
        {
            j = 0;
        }

        if (j == whatToFind.length())
        {
            break;
        }
    }

    return (j == whatToFind.length());
}

float StringUtils::ToFloat(const std::string &str)
{
    std::string number = str;
    StringUtils::Trim(&number);
    std::istringstream iss(number);
    float v;
    iss >> v;
    return v;
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

std::vector<int> StringUtils::BangEnumVariadicStringToValuesArray(const std::string &_va_args_)
{
    // We receive something like "Wololo, Apple = 49, Pear=29, Lololo=2193, Banana,Sandwich, Monkey=32"
    // We want this vector: [0, 49, 29, 2193, 2194, 2195, 32]
    std::vector<int> result;
    std::vector<std::string> splitted = SplitTrim(_va_args_, ',');
    int lastValue = 0;
    for (std::string str : splitted)
    {
        Trim(&str);
        std::vector<std::string> equalSplitted = SplitTrim(str, '=');
        int val;
        if (equalSplitted.size() == 2) // Has value
        {
            val = std::atoi(equalSplitted[1].c_str());
        }
        else // Has no value, one more than the last one
        {
            val = lastValue + 1;
        }

        lastValue = val;
        result.push_back(val);
    }
    return result;
}

std::string StringUtils::FormatInspectorLabel(const std::string &labelString)
{
    std::string labelFormatted = labelString;
    StringUtils::Replace(&labelFormatted, "_", " ");

    //Add space before any cap that does not have a space to its left
    for (auto it = labelFormatted.begin(); it != labelFormatted.end(); ++it)
    {
        if (IsCap(*it) && it != labelFormatted.begin())
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
