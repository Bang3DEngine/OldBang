#include "Bang/XMLParser.h"

#include "Bang/Paths.h"
#include "Bang/File.h"
#include "Bang/SerializableObject.h"


XMLParser::XMLParser()
{
}

String XMLParser::GetTagName(const String &tag,
                             int *tagNameBegin,
                             int *tagNameEnd)
{
    const String TokenSpace = " \t\n\r";
    int tagBegin = tag.IndexOfOneOf("<");
    int tagBegin2 = tag.IndexOfOneOf("/", tagBegin);
    tagBegin = (tagBegin2 == tagBegin + 1) ? tagBegin2 : tagBegin;
    int nameBegin = tag.IndexOfOneNotOf(TokenSpace, tagBegin + 1);
    int nameEnd = tag.IndexOfOneOf(TokenSpace + ">", nameBegin + 1);

    if (tagNameBegin) *tagNameBegin = nameBegin;
    if (tagNameEnd) *tagNameEnd = nameEnd;

    return tag.SubString(nameBegin, nameEnd - 1);
}

void XMLParser::GetFirstAttribute(const String &tag,
                                  int startPosition,
                                  XMLAttribute *attribute,
                                  int *attributeEnd)
{
    if (attributeEnd) *attributeEnd = -1;
    if (attribute)
    {
        attribute->SetName("");
        attribute->SetValue("");
    }

    int attrEnd = tag.IndexOf("}", startPosition);
    if (attrEnd == -1) { return; }

    String attrString = tag.SubString(startPosition, attrEnd);

    XMLAttribute attr = XMLAttribute::FromString(attrString);
    if (attr.GetName().Empty()) { return; }

    if (attributeEnd)
    {
        *attributeEnd = startPosition + attrString.Size();
    }

    if (attribute)
    {
        *attribute = attr;
    }
}

bool XMLParser::IsOpenTag(const String &tag)
{
    return tag[0] == '<' && tag[1] != '/';
}

void XMLParser::GetCorrespondingCloseTag(const String &xml,
                                         int startPositionAfterOpenTag,
                                         String tagName,
                                         int *beginPosition,
                                         int *endPosition)
{
    *beginPosition = -1;
    *endPosition   = -1;

    String resultTag = "";
    int end = startPositionAfterOpenTag;

    uint tagDeepness = 0;
    while (end != -1)
    {
        String tag;
        int begin;
        XMLParser::GetNextTag(xml, end, &tag, &begin, &end);
        if (end != -1)
        {
            String name = XMLParser::GetTagName(tag);
            if (name == tagName)
            {
                bool openTag = XMLParser::IsOpenTag(tag);
                if (openTag)
                {
                    ++tagDeepness;
                }
                else // closeTag
                {
                    if (tagDeepness == 0)
                    {
                        // We found the corresponding tag (same deepness level)
                        resultTag = tag;
                        *beginPosition = begin;
                        *endPosition = end;
                        break;
                    }
                    else
                    {
                        --tagDeepness;
                    }
                }
            }
        }
    }
}

void XMLParser::GetNextOpenTag(const String &xml,
                               int startPosition,
                               String *tag,
                               int *beginTagPosition,
                               int *endTagPosition)
{
    String resultTag = "";
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

void XMLParser::GetNextTag(const String &xml,
                           int startPosition,
                           String *tag,
                           int *beginPosition,
                           int *endTagPosition)
{
    *tag = "";
    *beginPosition  = -1;
    *endTagPosition = -1;

    int tagBegin = xml.IndexOf('<', startPosition);
    if (tagBegin == -1) { return; }

    int tagEnd = xml.IndexOf('>', tagBegin + 1);
    if (tagEnd == -1) { return; }

    if (tagBegin < tagEnd)
    {
        *tag = xml.SubString(tagBegin, tagEnd + 1);
        *beginPosition  = tagBegin;
        *endTagPosition = tagEnd;
    }
}

XMLNode XMLParser::FromFile(const Path &filepath)
{
    if (!filepath.IsFile()) { return XMLNode(); }
    String fileContents = File::GetContents(filepath);
    return XMLParser::FromString(fileContents);
}

XMLNode XMLParser::FromString(const String &xml)
{
    if (xml.Empty()) { return XMLNode(); }

    //Read name
    String tag;
    int rootOpenTagBegin, rootOpenTagEnd;
    XMLParser::GetNextOpenTag(xml, 0, &tag,
                              &rootOpenTagBegin, &rootOpenTagEnd);
    if (rootOpenTagEnd == -1) { return XMLNode(); }

    int tagNameEnd;
    String tagName = XMLParser::GetTagName(tag, nullptr, &tagNameEnd);
    int rootCloseTagBegin, rootCloseTagEnd;
    XMLParser::GetCorrespondingCloseTag(xml, rootOpenTagEnd, tagName,
                                        &rootCloseTagBegin, &rootCloseTagEnd);

    XMLNode root;
    root.SetTagName(tagName);

    //Read attributes
    int attrEnd = tagNameEnd;
    while (attrEnd != -1)
    {
        XMLAttribute attr;
        XMLParser::GetFirstAttribute(tag, attrEnd + 1, &attr, &attrEnd);
        if(attrEnd == -1) { break; }
        root.Set(attr.GetName(), attr.GetStringValue(), attr.GetProperties());
    }

    //Read children
    String innerXML = xml.SubString(rootOpenTagEnd + 1, rootCloseTagBegin - 1);
    int innerLastPos = 0;
    while (true)
    {
        String innerTag;
        int childOpenTagBegin, childOpenTagEnd;
        XMLParser::GetNextOpenTag(innerXML, innerLastPos, &innerTag,
                                  &childOpenTagBegin, &childOpenTagEnd);

        if (childOpenTagBegin == -1) { break; }

        String tagName = XMLParser::GetTagName(innerTag);
        int childCloseTagBegin, childCloseTagEnd;
        XMLParser::GetCorrespondingCloseTag(innerXML, childOpenTagEnd, tagName,
                                            &childCloseTagBegin, &childCloseTagEnd);
        String childXML = innerXML.SubString(childOpenTagBegin, childCloseTagEnd);

        XMLNode child = XMLParser::FromString(childXML);
        root.AddChild(child);

        innerLastPos = childCloseTagEnd;
    }

    return root;
}
