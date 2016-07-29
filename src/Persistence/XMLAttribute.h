#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang.h"

#include <string>
#include <vector>
#include <iostream>

#include "Rect.h"
#include "Vector3.h"
#include "IToString.h"
#include "Quaternion.h"
#include "StringUtils.h"
#include "XMLProperty.h"

class XMLAttribute : public IToString
{
public:

    enum Type
    {
        TBool = 0,
        TInt,
        TString,
        TFloat,
        TVector2,
        TVector3,
        TVector4,
        TQuaternion,
        TRect,
        TFile
    };

    static const std::vector< std::string > TypeNames;

    static XMLAttribute::Type GetTypeFromString(const std::string &typeString);

private:
    std::string m_name = "";
    XMLAttribute::Type m_type = XMLAttribute::Type::TBool;
    std::string m_value = "";
    std::vector<XMLProperty> m_properties;

public:
    XMLAttribute();
    XMLAttribute(const std::string &name,
                 const std::string &value,
                 XMLAttribute::Type type,
                 const std::vector<XMLProperty> &properties = {});

    void Set(const std::string &name,
             const std::string &value,
             XMLAttribute::Type type,
             const std::vector<XMLProperty> &properties = {});
    void SetName(const std::string &name);
    void SetType(const XMLAttribute::Type &type);
    void SetValue(const std::string &value);
    void SetProperty(const XMLProperty &property);
    void SetProperty(const std::string &propertyName, const std::string &propertyValue = "");
    void SetProperties(const std::vector<XMLProperty>& properties);
    const std::string GetPropertyValue(const std::string &propertyName);
    bool HasProperty(const XMLProperty &property) const;
    bool HasProperty(const std::string &propertyName) const;
    void RemoveProperty(const std::string &propertyName);


    void SetPointer(const void *value, const std::vector<XMLProperty>& properties = {});
    void SetBool(bool value, const std::vector<XMLProperty>& properties = {});
    void SetInt(int value, const std::vector<XMLProperty>& properties = {});
    void SetString(const std::string &value, const std::vector<XMLProperty>& properties = {});
    void SetFloat(float value, const std::vector<XMLProperty>& properties = {});
    void SetVector2(const glm::vec2 &value, const std::vector<XMLProperty>& properties = {});
    void SetVector3(const Vector3 &value, const std::vector<XMLProperty>& properties = {});
    void SetVector4(const glm::vec4 &value, const std::vector<XMLProperty>& properties = {});
    void SetQuaternion(const Quaternion &value, const std::vector<XMLProperty>& properties = {});
    void SetRect(const Rect &value, const std::vector<XMLProperty>& properties = {});
    void SetFilepath(const std::string &filepath, const std::string &fileExtension = "",
                     const std::vector<XMLProperty>& properties = {});

    const std::string ToString() const override;

    const std::string& GetName() const;
    const XMLAttribute::Type& GetType() const;

    const std::string& GetValue() const;
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    std::string GetFilepath() const;
    std::string GetString() const;
    glm::vec2 GetVector2() const;
    Vector3 GetVector3() const;
    glm::vec4 GetVector4() const;
    Quaternion GetQuaternion() const;
    Rect GetRect() const;

    const std::string& GetTypeName() const;
    const std::vector<XMLProperty>& GetProperties() const;
};

#endif // XMLATTRIBUTE_H
