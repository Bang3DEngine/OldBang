#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include <string>

class XMLAttribute
{
public:
    enum Type
    {
        Bool = 0,
        Int,
        String,
        Float,
        Vector2,
        Vector3,
        Vector4,
        Quaternion,
        Rect,
        Enum,
        File
    };

    static const std::string TypeNames[11];

private:
    std::string m_name = "";
    std::string m_value = "";
    XMLAttribute::Type m_type = XMLAttribute::Type::Bool;

public:
    XMLAttribute();
    XMLAttribute(const std::string &name,
                 const std::string &value,
                 XMLAttribute::Type type);

    void SetName(const std::string &name);
    void SetValue(const std::string &value);
    void SetType(const XMLAttribute::Type &type);

    const std::string& GetName() const;
    const std::string& GetValue() const;
    const XMLAttribute::Type& GetType() const;
    const std::string& GetTypeName() const;

};

#endif // XMLATTRIBUTE_H
