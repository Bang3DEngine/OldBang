#include "StringUtils.h"



void StringUtils::AddInFrontOfWords(std::string particle, std::string *str)
{
    std::string &phrase = *str;
    if(phrase.length() > 0 && phrase[0] != ' ')
    {
        phrase.insert(0, particle);
    }

    for(int i = 0; i < phrase.length() -1; ++i)
    {
        if(phrase[i] == ' ' && phrase[i+1] != ' ')
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
