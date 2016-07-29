#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include <string>
#include <vector>
#include <iostream>

#include "IToString.h"

class XMLAttribute : public IToString
{
public:

    const static std::string PropertyHidden;
    const static std::string PropertyReadonly;

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
        File
    };

    static const std::vector< std::string > TypeNames;

    static XMLAttribute::Type GetTypeFromString(const std::string &typeString);

private:
    std::string m_name = "";
    XMLAttribute::Type m_type = XMLAttribute::Type::Bool;
    std::string m_value = "";
    std::vector<std::string> m_properties;

public:
    XMLAttribute();
    XMLAttribute(const std::string &name,
                 const std::string &value,
                 XMLAttribute::Type type,
                 const std::vector<std::string> &properties = {});

    void SetName(const std::string &name);
    void SetType(const XMLAttribute::Type &type);
    void SetValue(const std::string &value);
    void SetProperty(const std::string &property);
    bool HasProperty(const std::string &property) const;
    void RemoveProperty(const std::string &property);

    const std::string ToString() const override;

    const std::string& GetName() const;
    const XMLAttribute::Type& GetType() const;
    const std::string& GetTypeName() const;
    const std::string& GetValue() const;
    const std::vector<std::string>& GetProperties() const;
};

#endif // XMLATTRIBUTE_H
