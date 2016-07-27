#include "XMLParser.h"

const std::string XMLParser::TOKEN_SPACE = " \t\n";

XMLParser::XMLParser()
{
}

std::string XMLParser::GetTagName(const std::string &tag, int *tagNameBegin, int *tagNameEnd)
{
    int tagBegin = tag.find_first_of('<');
    int nameBegin = tag.find_first_not_of(TOKEN_SPACE, tagBegin + 1);
    int nameEnd = tag.find_first_of(TOKEN_SPACE + ">/", nameBegin + 1);

    if (tagNameBegin) *tagNameBegin = nameBegin;
    if (tagNameEnd) *tagNameEnd = nameEnd;

    return tag.substr(nameBegin, nameEnd - nameBegin);
}

void XMLParser::GetFirstAttribute(const std::string &tag,
                                  int startPosition,
                                  std::pair<std::string, std::string> *attribute,
                                  int *attributeEnd)
{
    if (attributeEnd) *attributeEnd = -1;
    if (attribute)
    {
        attribute->first  = "";
        attribute->second = "";
    }

    int attrNameBegin = tag.find_first_not_of(TOKEN_SPACE, startPosition);
    if (attrNameBegin == -1) { return; }

    int attrNameEnd = tag.find_first_of(TOKEN_SPACE + "=", attrNameBegin + 1);
    if (attrNameEnd == -1) { return; }

    int attrValueBegin = tag.find_first_of("\"", attrNameEnd + 1) + 1;
    if (attrValueBegin == -1) { return; }

    int attrValueEnd = tag.find_first_of("\"", attrValueBegin + 1);
    if (attrValueEnd == -1) { return; }

    std::string name = tag.substr(attrNameBegin, attrNameEnd - attrNameBegin);
    std::string value = tag.substr(attrValueBegin, attrValueEnd - attrValueBegin);

    if (attributeEnd) *attributeEnd = attrValueEnd;
    if (attribute)
    {
        attribute->first  = name;
        attribute->second = value;
    }
}

bool XMLParser::IsOpenTag(const std::string &tag)
{
    return tag[tag.length()-2] != '/' && tag[tag.length()-1] == '>';
}

void XMLParser::GetCloseTag(const std::string &xml,
                            int startPosition,
                            std::string tagName,
                            int *beginPosition,
                            int *endPosition)
{
    *beginPosition = -1;
    *endPosition   = -1;

    std::string resultTag = "";
    int end = startPosition;
    int resultBegin = -1, resultEnd = -1;
    while (true)
    {
        std::string tag;
        int begin;
        XMLParser::GetNextTag(xml, end, &tag, &begin, &end);
        if (end != -1 && !XMLParser::IsOpenTag(tag))
        {
            std::string name = XMLParser::GetTagName(tag);
            if (name == tagName)
            {
                // Keep the search, we are looking for the last one
                resultTag = tag;
                resultBegin = begin;
                resultEnd = end;
            }
        }

        if (end == -1)
        {
            break;
        }
    }

    *beginPosition = resultBegin;
    *endPosition = resultEnd;
}

void XMLParser::GetNextOpenTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginTagPosition,
                               int *endTagPosition)
{
    std::string resultTag = "";
    int end = startPosition;
    while (end != -1)
    {
        XMLParser::GetNextTag(xml, end, &resultTag, beginTagPosition, &end);
        if (XMLParser::IsOpenTag(resultTag))
        {
            break;
        }
    }

    if (end == -1)
    {
        *tag = "";
        *beginTagPosition = -1;
        *endTagPosition = -1;
    }
    else
    {
        *tag = resultTag;
        *endTagPosition = end;
    }
}

void XMLParser::GetNextTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginPosition,
                               int *endTagPosition)
{
    *tag = "";
    *beginPosition  = -1;
    *endTagPosition = -1;

    int tagBegin = xml.find_first_of('<', startPosition);
    if (tagBegin == -1) { return; }

    int tagEnd = xml.find_first_of('>', tagBegin + 1);
    if (tagEnd == -1) { return; }

    if (tagBegin < tagEnd)
    {
        *tag = xml.substr(tagBegin, tagEnd-tagBegin+1);
        *beginPosition  = tagBegin;
        *endTagPosition = tagEnd;
    }
}

XMLNode* XMLParser::FromXML(const std::string &xml)
{
    XMLNode* root = new XMLNode();

    //Read name
    std::string tag;
    int rootOpenTagBegin, rootOpenTagEnd;
    XMLParser::GetNextOpenTag(xml, 0, &tag,
                              &rootOpenTagBegin, &rootOpenTagEnd);
    if (rootOpenTagEnd == -1)
    {
        return nullptr;
    }

    int tagNameEnd;
    std::string tagName = XMLParser::GetTagName(tag, nullptr, &tagNameEnd);
    int rootCloseTagBegin, rootCloseTagEnd;
    XMLParser::GetCloseTag(xml, rootOpenTagEnd, tagName,
                           &rootCloseTagBegin, &rootCloseTagEnd);

    root->SetTagName(tagName);

    //Read attributes
    int attrEnd = tagNameEnd;
    while (attrEnd != -1)
    {
        //Logger_Log("attrEnd: " << attrEnd);
        std::pair<std::string, std::string> attr;
        XMLParser::GetFirstAttribute(tag, attrEnd + 1, &attr, &attrEnd);
        if(attrEnd == -1)
        {
            break;
        }

        root->AddAttribute(attr.first, attr.second);
    }

    //Read children
    std::string innerXML = xml.substr(rootOpenTagEnd+1, rootCloseTagBegin-rootOpenTagEnd-1);
    int innerLastPos = 0;
    while (true)
    {
        std::string innerTag;
        int childOpenTagBegin, childOpenTagEnd;
        XMLParser::GetNextTag(innerXML, innerLastPos, &innerTag,
                              &childOpenTagBegin, &childOpenTagEnd);
        if (childOpenTagBegin == -1)
        {
            break;
        }

        std::string tagName = XMLParser::GetTagName(innerTag);
        int childCloseTagBegin, childCloseTagEnd;
        XMLParser::GetCloseTag(innerXML, innerLastPos, tagName,
                               &childCloseTagBegin, &childCloseTagEnd);

        std::string childXML = innerXML.substr(childOpenTagBegin, childCloseTagEnd-childOpenTagBegin);

        XMLNode *child = XMLParser::FromXML(childXML);
        if (child)
        {
            root->AddChild(child);
        }

        innerLastPos = childCloseTagEnd;
    }

    return root;
}
