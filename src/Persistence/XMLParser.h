#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <sstream>

#include "XMLNode.h"

class XMLParser
{
public:
    static const std::string TOKEN_SPACE;

private:
    static void GetNextOpenTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginPosition,
                               int *endTagPosition);

    static void GetNextTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginPosition,
                               int *endTagPosition);

    static void GetCloseTag(const std::string &xml,
                            int startPosition,
                            std::string tagName,
                            int *beginPosition,
                            int *endPosition);

    static void GetFirstAttribute(const std::string &tag,
                                  int startPosition,
                                  std::pair<std::string, std::string> *attribute,
                                  int *attributeEnd);

    static bool IsOpenTag(const std::string &tag);

public:
    XMLParser();

    static std::string GetTagName(const std::string &tag,
                                  int *tagNameBegin = nullptr, int *tagNameEnd = nullptr);
    static XMLNode* FromXML(const std::string &xml);
};

#endif // XMLPARSER_H
