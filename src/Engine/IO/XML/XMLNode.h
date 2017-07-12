#ifndef XMLNODE_H
#define XMLNODE_H

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Rect.h"
#include "Bang/Vector3.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"
#include "Bang/XMLAttribute.h"

class XMLNode : public IToString
{
public:
    XMLNode(const String &tagName = "");
    virtual ~XMLNode();

    void CloneInto(XMLNode *xmlNode) const;

    void AddChild(const XMLNode &node);

    void UpdateAttributeValue(const String &attributeName,
                              const String &newAttributeValue);

    void SetAttribute(const XMLAttribute &attribute);
    void SetGenericAttribute(const String &attributeName,
                             const String &attributeValue,
                             XMLAttribute::Type type,
                             const Array<XMLProperty>& properties = {});
    void SetPointer(const String &attributeName,
                    const void *value,
                    const Array<XMLProperty>& properties = {});
    void SetBool(const String &attributeName, bool value,
                 const Array<XMLProperty>& properties = {});
    void SetInt(const String &attributeName, int value,
                const Array<XMLProperty>& properties = {});
    void SetFloat(const String &attributeName, float value,
                  const Array<XMLProperty>& properties = {});
    void SetString(const String &attributeName, const String &value,
                   const Array<XMLProperty>& properties = {});
    void SetVector2(const String &attributeName, const Vector2 &value,
                    const Array<XMLProperty>& properties = {});
    void SetVector3(const String &attributeName, const Vector3 &value,
                    const Array<XMLProperty>& properties = {});
    void SetVector4(const String &attributeName, const Vector4 &value,
                    const Array<XMLProperty>& properties = {});
    void SetColor(const String &attributeName, const Color &value,
                  const Array<XMLProperty>& properties = {});
    void SetQuaternion(const String &attributeName, const Quaternion &value,
                       const Array<XMLProperty>& properties = {});
    void SetRect(const String &attributeName, const Rect &value,
                 const Array<XMLProperty>& properties = {});
    void SetFilepath(const String &attributeName,
                     const Path &filepath,
                     const Array<XMLProperty>& properties = {});

    void RemoveAttribute(const String& attributeName);
    XMLAttribute* GetAttribute(const String& attributeName) const;
    String GetAttributeValue(const String& attributeName) const;

    bool GetBool(const String& attributeName) const;
    int GetInt(const String& attributeName) const;
    float GetFloat(const String& attributeName) const;
    Path GetFilepath(const String& attributeName) const;
    String GetString(const String& attributeName) const;
    Vector2 GetVector2(const String& attributeName) const;
    Vector3 GetVector3(const String& attributeName) const;
    Vector4 GetVector4(const String& attributeName) const;
    Color GetColor(const String& attributeName) const;
    Quaternion GetQuaternion(const String& attributeName) const;
    Rect GetRect(const String& attributeName) const;

    template<class EnumClass>
    void SetEnum(const String &attributeName,
                 const EnumClass value,
                 const Array<XMLProperty>& properties = {})
    {
        XMLAttribute attr;
        attr.SetName(attributeName);
        attr.SetEnum<EnumClass>(value, properties);
        SetAttribute(attr);
    }

    template<class EnumClass>
    EnumClass GetEnum(const String& attributeName) const
    {
        XMLAttribute *attr = GetAttribute(attributeName);
        return attr->GetEnum<EnumClass>();
    }

    const XMLNode *GetChild(const String &name) const;
    void SetTagName(const String tagName);
    String ToString() const override;
    String ToString(const String& indent) const;

    const String& GetTagName() const;
    const Map<String, XMLAttribute>& GetAttributes() const;
    const List<String>& GetAttributesOrderList() const;
    List< std::pair<String, XMLAttribute> > GetAttributesListInOrder() const;
    const List<XMLNode>& GetChildren() const;
    List<XMLNode>& GetChildren();

    static XMLNode FromString(const String &xml);

private:
    String m_tagName = "";
    mutable List<String> m_attributeOrder;
    mutable Map<String, XMLAttribute> m_attributes;
    List<XMLNode> m_children;
};

#endif // XMLNODE_H
