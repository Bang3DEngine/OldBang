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
#include "Bang/OStreamOperators.h"

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

    template<class T>
    void SetVector2(const Vector2G<T> &value, const Array<XMLProperty>& properties = {})
    {
        std::ostringstream oss;
        oss << value;
        Set(m_name, oss.str(), XMLAttribute::Type::Vector2, properties);
    }

    template<class T>
    void SetVector3(const Vector3G<T> &value, const Array<XMLProperty>& properties = {})
    {
        std::ostringstream oss; oss << value;
        Set(m_name, oss.str(), XMLAttribute::Type::Vector3, properties);
    }

    template<class T>
    void SetVector4(const Vector4G<T> &value, const Array<XMLProperty>& properties = {})
    {
        std::ostringstream oss; oss << value;
        Set(m_name, oss.str(), XMLAttribute::Type::Vector4, properties);
    }
    template<class T>
    void SetQuaternion(const QuaternionG<T> &value, const Array<XMLProperty>& properties = {})
    {
        std::ostringstream oss; oss << value;
        Set(m_name, oss.str(), XMLAttribute::Type::Quaternion, properties);
    }

    template<class T>
    void SetRect(const RectG<T> &value, const Array<XMLProperty>& properties = {})
    {
        std::ostringstream oss; oss << value;
        Set(m_name, oss.str(), XMLAttribute::Type::Rect, properties);
    }

    void SetColor(const Color &value, const Array<XMLProperty>& properties = {});
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
    Color GetColor() const;

    template<class T = float>
    Vector2G<T> GetVector2() const
    {
        T x = 0, y = 0;
        String insidePars = m_value.Split('(')[1];
        insidePars =  insidePars.Split(')')[0];
        Array<String> numbers = insidePars.Split(',');
        x = String::To<T>(numbers[0]);
        y = String::To<T>(numbers[1]);
        return Vector2G<T>(x,y);
    }

    template<class T = float>
    Vector3G<T> GetVector3() const
    {
        T x = 0, y = 0, z = 0;
        String insidePars = m_value.Split('(')[1];
        insidePars =  insidePars.Split(')')[0];
        Array<String> numbers = insidePars.Split(',');
        x = String::To<T>(numbers[0]);
        y = String::To<T>(numbers[1]);
        z = String::To<T>(numbers[2]);
        return Vector3G<T>(x,y,z);
    }

    template<class T = float>
    Vector4G<T> GetVector4() const
    {
        T x = 0, y = 0, z = 0, w = 0;
        Array<String> arr = m_value.Split('(');
        String insidePars = arr[1];
        arr = insidePars.Split(')');
        insidePars =  arr[0];
        Array<String> numbers = insidePars.Split(',');
        x = String::To<T>(numbers[0]);
        y = String::To<T>(numbers[1]);
        z = String::To<T>(numbers[2]);
        w = String::To<T>(numbers[3]);
        return Vector4G<T>(x,y,z,w);
    }


    template<class T = float>
    QuaternionG<T> GetQuaternion() const
    {
        Vector4G<T> v = GetVector4<T>();
        return QuaternionG<T>(v.w, v.x, v.y, v.z);
    }

    template<class T = float>
    RectG<T> GetRect() const
    {
        Vector4G<T> v = GetVector4<T>();
        return RectG<T>(v.x, v.y, v.z, v.w);
    }


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
