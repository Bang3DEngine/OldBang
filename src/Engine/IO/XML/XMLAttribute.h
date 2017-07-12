#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang/Path.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/NamedEnum.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"
#include "Bang/XMLProperty.h"

class XMLAttribute : public IToString
{
public:
    NamedEnum( Type,
        Bool = 0,
        Int,
        String,
        Float,
        Vector2,
        Vector3,
        Vector4,
        Color,
        Quaternion,
        Rect,
        File,
        Enum
    );

    XMLAttribute();
    XMLAttribute(const String &name,
                 const String &value,
                 XMLAttribute::Type type,
                 const Array<XMLProperty> &properties = {});

    void Set(const String &name,
             const String &value,
             XMLAttribute::Type type,
             const Array<XMLProperty> &properties = {});
    void SetName(const String &name);
    void SetType(const XMLAttribute::Type &type);
    void SetValue(const String &value);
    void SetProperty(const XMLProperty &property);
    void SetProperty(const String &propertyName, const String &propertyValue = "");
    void SetProperties(const Array<XMLProperty>& properties);
    const String& GetPropertyValue(const String &propertyName) const;
    bool HasProperty(const XMLProperty &property) const;
    bool HasProperty(const String &propertyName) const;
    void RemoveProperty(const String &propertyName);

    void SetPointer(const void *value, const Array<XMLProperty>& properties = {});
    void SetBool(bool value, const Array<XMLProperty>& properties = {});
    void SetInt(int value, const Array<XMLProperty>& properties = {});
    void SetString(const String &value, const Array<XMLProperty>& properties = {});
    void SetFloat(float value, const Array<XMLProperty>& properties = {});
    void SetVector2(const Vector2 &value, const Array<XMLProperty>& properties = {});
    void SetVector3(const Vector3 &value, const Array<XMLProperty>& properties = {});
    void SetVector4(const Vector4 &value, const Array<XMLProperty>& properties = {});
    void SetColor(const Color &value, const Array<XMLProperty>& properties = {});
    void SetQuaternion(const Quaternion &value, const Array<XMLProperty>& properties = {});
    void SetRect(const Rect &value, const Array<XMLProperty>& properties = {});
    void SetFilepath(const Path &filepath,
                     const Array<XMLProperty>& properties = {});

    String ToString() const override;

    const String& GetName() const;
    const XMLAttribute::Type& GetType() const;

    String GetValue() const;
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    Path GetFilepath() const;
    String GetString() const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;
    Color GetColor() const;
    Quaternion GetQuaternion() const;
    Rect GetRect() const;

    template<class EnumClass>
    void SetEnum(EnumClass value, const Array<XMLProperty> &properties)
    {
        Set(m_name, String::ToString( static_cast<int>(value) ),
            XMLAttribute::Type::Enum, properties);
    }

    template<class EnumClass>
    EnumClass GetEnum() const
    {
        return static_cast<EnumClass>(String::ToInt(GetValue().ToCString()));
    }

    String GetTypeName() const;
    const Array<XMLProperty>& GetProperties() const;

    static XMLAttribute FromString(const String &string);

private:
    String m_name = "";
    XMLAttribute::Type m_type = XMLAttribute::Type::Bool;
    String m_value = "";
    Array<XMLProperty> m_properties;

};

#endif // XMLATTRIBUTE_H
