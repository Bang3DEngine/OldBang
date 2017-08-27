#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang/Paths.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/XMLProperty.h"
#include "Bang/StreamOperators.h"

class XMLAttribute : public IToString
{
public:
    XMLAttribute();
    XMLAttribute(const String &name,
                 const String &value,
                 const Array<XMLProperty> &properties = {});

    void Set(const String &name,
             const String &value,
             const Array<XMLProperty> &properties = {});

    void SetProperty(const XMLProperty &property);
    void SetProperty(const String &propertyName, const String &propertyValue = "");
    void SetProperties(const Array<XMLProperty>& properties);
    const String& GetPropertyValue(const String &propertyName) const;
    bool HasProperty(const XMLProperty &property) const;
    bool HasProperty(const String &propertyName) const;
    void RemoveProperty(const String &propertyName);
    const Array<XMLProperty>& GetProperties() const;

    void SetName(const String &name);
    const String& GetName() const;

    void SetValue(const String &value);
    const String& GetStringValue() const;

    String ToString() const;
    static XMLAttribute FromString(const String &string);

    template<class T>
    void Set(const String &name,
             const T& value,
             const Array<XMLProperty> &properties = {})
    {
        std::ostringstream oss; oss << value;
        Set(name, String(oss.str()), properties);
    }

    template<class T>
    T Get() const
    {
        T t;
        std::istringstream iss(GetStringValue());
        iss >> t;
        return t;
    }

protected:
    String m_name  = "";
    String m_value = "";
    Array<XMLProperty> m_properties;
};


template<>
inline void XMLAttribute::Set(const String &name,
                              const Path& filepath,
                              const Array<XMLProperty> &properties)
{
    String relFilepath = Paths::GetRelative(filepath).ToString();
    Set(name, relFilepath, properties);

    if (Paths::IsEnginePath(filepath))
    {
        SetProperty(XMLProperty::IsEngineFile);
    }
    else
    {
        RemoveProperty(XMLProperty::IsEngineFile.GetName());
    }
}

template<>
inline void XMLAttribute::Set(const String &name,
                              const bool& value,
                              const Array<XMLProperty> &properties)
{
    Set(name, value ? "True" : "False", properties);
}

template<>
inline bool XMLAttribute::Get() const
{
    return GetStringValue().EqualsNoCase("true");
}

template<>
inline Path XMLAttribute::Get() const
{
    if ( GetStringValue().IsEmpty() ) { return Path::Empty; }
    return HasProperty(XMLProperty::IsEngineFile) ?
                EPATH(GetStringValue()) :
                PPATH(GetStringValue());
}

#endif // XMLATTRIBUTE_H
