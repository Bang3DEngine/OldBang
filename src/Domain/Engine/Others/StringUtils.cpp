#include "StringUtils.h"

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
    unsigned int i = 0;
    for (; i < str->length(); ++i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(i, str->length() - i);
}

void StringUtils::TrimRight(std::string *str)
{
    unsigned int i = str->length()-1;
    for (; i >= 0; --i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(0, i-1);
}

void StringUtils::Trim(std::string *str)
{
    StringUtils::TrimLeft(str);
    StringUtils::TrimRight(str);
}


std::vector<std::string> StringUtils::Split(const std::string &content, char splitter)
{
    std::vector<std::string> result;
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
