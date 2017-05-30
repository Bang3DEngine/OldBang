#ifndef BANGPREPROCESSOR_H
#define BANGPREPROCESSOR_H

#include "Bang/Array.h"
#include "Bang/String.h"

class BangPreprocessor
{
public:
    static String Preprocess(const String &source);

public:
    const static Array<String> Modifiers;
    const static Array<String> VarTypes;
    const static Array<String> PropertyPrefixes;
    const static Array<String> StructPrefixes;

    static void RemoveComments(String *source);

    static String::Iterator Find(String::Iterator begin,
                                 String::Iterator end,
                                 const Array<String> &toFindList);

    static void GetNextScope(String::Iterator begin,
                             String::Iterator end,
                             String::Iterator *scopeBegin,
                             String::Iterator *scopeEnd);

    static void SkipBlanks(String::Iterator *it,
                           String::Iterator end);
    static void SkipUntilNewBlank(String::Iterator *it,
                                  String::Iterator end);

    static void FindNextWord(String::Iterator begin,
                             String::Iterator end,
                             String::Iterator *wordBegin,
                             String::Iterator *wordEnd);

    BangPreprocessor();
};

#endif // BANGPREPROCESSOR_H
