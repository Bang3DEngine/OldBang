#include "XMLParser.h"

#include "Debug.h"
#include "IFileable.h"
#include "Persistence.h"

Map<String, const IFileable*> XMLParser::m_id_To_Pointer;

XMLParser::XMLParser()
{
}

void XMLParser::RegisterId(const XMLNode *xmlInfo, const IFileable *pointer)
{
    String id = xmlInfo->GetString("id");
    m_id_To_Pointer[id] = pointer;
}

const IFileable *XMLParser::GetPointerFromId(const String &id)
{
    if (m_id_To_Pointer.ContainsKey(id))
    {
        return m_id_To_Pointer[id];
    }
    return nullptr;
}

String XMLParser::GetTagName(const String &tag, int *tagNameBegin, int *tagNameEnd)
{
    int tagBegin = tag.IndexOfOneOf("<");
    int tagBegin2 = tag.IndexOfOneOf("/", tagBegin);
    tagBegin = (tagBegin2 == tagBegin + 1) ? tagBegin2 : tagBegin;
    int nameBegin = tag.IndexOfOneNotOf(StringUtils::TOKEN_SPACE, tagBegin + 1);
    int nameEnd = tag.IndexOfOneOf(StringUtils::TOKEN_SPACE + ">", nameBegin + 1);

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
        attribute->SetType(XMLAttribute::Type::Bool); // For example
        attribute->SetValue("");
    }

    int attrEnd = tag.IndexOf("}", startPosition);
    if (attrEnd == -1) { return; }

    String attrString = tag.SubString(startPosition, attrEnd);

    XMLAttribute attr = XMLAttribute::FromString(attrString);
    if (attr.GetName() == "") { return; }

    if (attributeEnd)
    {
        *attributeEnd = startPosition + attrString.Length();
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

    unsigned int tagDeepness = 0;
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

void XMLParser::ClearPointerIds()
{
    m_id_To_Pointer.Clear();
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

XMLNode *XMLParser::FromFile(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return nullptr);
    ASSERT(Persistence::ExistsFile(filepath), "", return nullptr);

    std::ifstream f;
    try
    {
        f.open(filepath);
    }
    catch (std::ifstream::failure)
    {
        return nullptr;
    }

    if (f.is_open())
    {
        String contents((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
        XMLNode *xmlInfo = XMLParser::FromString(contents);
        f.close();
        return xmlInfo;
    }
    return nullptr;
}

XMLNode* XMLParser::FromString(const String &xml)
{
    ClearPointerIds();

    XMLNode* root = new XMLNode();

    //Read name
    String tag;
    int rootOpenTagBegin, rootOpenTagEnd;
    XMLParser::GetNextOpenTag(xml, 0, &tag,
                              &rootOpenTagBegin, &rootOpenTagEnd);
    if (rootOpenTagEnd == -1) { delete root; return nullptr; }

    int tagNameEnd;
    String tagName = XMLParser::GetTagName(tag, nullptr, &tagNameEnd);
    int rootCloseTagBegin, rootCloseTagEnd;
    XMLParser::GetCorrespondingCloseTag(xml, rootOpenTagEnd, tagName,
                                        &rootCloseTagBegin, &rootCloseTagEnd);

    root->SetTagName(tagName);

    //Read attributes
    int attrEnd = tagNameEnd;
    while (attrEnd != -1)
    {
        XMLAttribute attr;
        XMLParser::GetFirstAttribute(tag, attrEnd + 1, &attr, &attrEnd);
        if(attrEnd == -1) { break; }
        root->SetGenericAttribute(attr.GetName(), attr.GetValue(),
                                  attr.GetType(), attr.GetProperties());
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

        XMLNode *child = XMLParser::FromString(childXML);
        if (child)
        {
            root->AddChild(child);
        }

        innerLastPos = childCloseTagEnd;
    }

    return root;
}
