#include "StringUtils.h"


std::vector<std::string> StringUtils::Split(const std::string &content, char splitter)
{
    std::vector<std::string> result;
    int lastIndexFound = 0;
    while (lastIndexFound != -1)
    {
        int indexFound = content.find_first_of(splitter, lastIndexFound - indexFound + 1);
        if (indexFound == std::string::npos)
        {
            break;
        }
        lastIndexFound = indexFound;
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
