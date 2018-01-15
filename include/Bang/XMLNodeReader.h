#ifndef XMLNODEREADER_H
#define XMLNODEREADER_H

#include <sstream>
#include <fstream>

#include "Bang/XMLNode.h"

NAMESPACE_BANG_BEGIN

FORWARD class Serializable;

class XMLNodeReader
{
public:
    static void GetNextOpenTag(const String &xml,
                               int startPosition,
                               String *tag,
                               int *beginPosition,
                               int *endTagPosition);

    static void GetNextTag(const String &xml,
                           int startPosition,
                           String *tag,
                           int *beginPosition,
                           int *endTagPosition);

    static void GetCorrespondingCloseTag(const String &xml,
                                         int startPositionAfterOpenTag,
                                         String tagName,
                                         int *beginPosition,
                                         int *endPosition);

    static void GetNextAttribute(const String &tag,
                                 int startPosition,
                                 XMLAttribute *attribute,
                                 int *attributeEnd);

    static bool IsOpenTag(const String &tag);

public:
    XMLNodeReader();

    static String GetTagName(const String &tag,
                             int *tagNameBegin = nullptr,
                             int *tagNameEnd = nullptr);

    static XMLNode FromFile(const Path &filepath);
    static XMLNode FromString(const String &xml);
};

NAMESPACE_BANG_END

#endif // XMLNODEREADER_H
