#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang/Array.h"

#include "Bang/NamedEnum.h"

#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"
#include "Bang/XMLProperty.h"

class IAttrWidgetButtonListener;
class XMLAttribute : public IToString
{
public:

    NamedEnum (Type,
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
        Enum,
        Button
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
    const String GetPropertyValue(const String &propertyName) const;
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
    void SetFilepath(const String &filepath, const String &fileExtension = "",
                     const Array<XMLProperty>& properties = {});
    void SetEnum(const Array<String>& enumNames,
                 const String& selectedEnumName,
                 const Array<XMLProperty>& properties = {});
    void SetEnum(const Array<String>& enumNames,
                 int selectedEnumIndex, const Array<XMLProperty>& properties = {});

    #ifdef BANG_EDITOR
    void SetButton(const String buttonText,
                   IAttrWidgetButtonListener *listener,
                   const Array<XMLProperty>& properties = {});
    #endif

    /**
     * @brief HasVectorLikeType
     * @return Whether the type is either Float, Vector*, or Quaternion.
     */
    bool HasVectoredType() const;

    /**
     * @brief GetNumberOfFields
     * @return The number of fields of the type. For example: TFloat=1, TVector2=2, TQuaternion=4, etc.
     */
    int GetNumberOfFieldsOfType() const;

    String ToString() const override;

    const String& GetName() const;
    const XMLAttribute::Type& GetType() const;

    String GetValue() const;
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    String GetFilepath() const;
    String GetString() const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;
    Color GetColor() const;
    Quaternion GetQuaternion() const;
    Rect GetRect() const;
    IAttrWidgetButtonListener* GetButtonListener() const;
    String GetEnumSelectedName() const;
    int GetEnumSelectedIndex() const;
    Array<String> GetEnumNames() const;

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
