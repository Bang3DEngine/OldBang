#ifndef BANGPREPROCESSOR_H
#define BANGPREPROCESSOR_H

#include "Bang/Array.h"
#include "Bang/String.h"

FORWARD class Path;

class BangPreprocessor
{
public:
    static void Preprocess(const Path &filepath);
    static void Preprocess(const String &source,
                           String *reflectionHeaderSource,
                           bool *preprocessedSomething);

public:
    const static Array<String> VarTypeInt;
    const static Array<String> VarTypeBool;
    const static Array<String> VarTypeFloat;
    const static Array<String> VarTypeColor;
    const static Array<String> VarTypeDouble;
    const static Array<String> VarTypeString;
    const static Array<String> VarTypeVector2;
    const static Array<String> VarTypeVector3;
    const static Array<String> VarTypeVector4;
    const static Array<String> VarTypeQuaternion;

    const static Array<String> Modifiers;
    const static Array<String> VarTypes;
    const static Array<String> RVariablePrefixes;
    const static Array<String> RStructPrefixes;
    const static String ReflectDefinitionsDefineName;
    const static String ReflectionInfoVarName;

    static void RemoveComments(String *source);

    static String::Iterator Find(String::Iterator begin,
                                 String::Iterator end,
                                 const Array<String> &toFindList);

    static void GetNextScope(String::Iterator begin,
                             String::Iterator end,
                             String::Iterator *scopeBegin,
                             String::Iterator *scopeEnd,
                             char openingBrace,
                             char closingBrace);

    static void SkipBlanks(String::Iterator *it,
                           String::Iterator end);
    static void SkipUntilNextBlank(String::Iterator *it,
                                   String::Iterator end);
    static void SkipUntilNext(String::Iterator *it,
                              String::Iterator end,
                              const Array<String> &particles);

    static void FindNextWord(String::Iterator begin,
                             String::Iterator end,
                             String::Iterator *wordBegin,
                             String::Iterator *wordEnd);

    BangPreprocessor();
};

#endif // BANGPREPROCESSOR_H
