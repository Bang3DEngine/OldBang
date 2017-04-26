#ifndef XMLPARSER_H
#define XMLPARSER_H


#include <sstream>
#include <fstream>

#include "Bang/XMLNode.h"

class SerializableObject;
class XMLParser
{
private:
public:
    static Map<String, const SerializableObject*> m_id_To_Pointer;

    static void ClearPointerIds();

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

    static void GetFirstAttribute(const String &tag,
                                  int startPosition,
                                  XMLAttribute *attribute,
                                  int *attributeEnd);

    static bool IsOpenTag(const String &tag);

public:
    XMLParser();

    static void RegisterId(const XMLNode *xmlInfo, const SerializableObject *pointer);
    static const SerializableObject* GetPointerFromId(const String &id);

    static String GetTagName(const String &tag,
                                  int *tagNameBegin = nullptr, int *tagNameEnd = nullptr);

    static XMLNode FromFile(const String &filepath);
    static XMLNode FromString(const String &xml);
};

#endif // XMLPARSER_H
