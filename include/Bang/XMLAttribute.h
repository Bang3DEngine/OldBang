#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang/Paths.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/StreamOperators.h"

NAMESPACE_BANG_BEGIN

class XMLAttribute : public IToString
{
public:
    XMLAttribute();
    XMLAttribute(const String &name, const String &value);

    void Set(const String &name, const String &value);

    void SetName(const String &name);
    const String& GetName() const;

    void SetValue(const String &value);
    const String& GetStringValue() const;

    String ToString() const;
    static XMLAttribute FromString(const String &string);

    template<class T>
    void Set(const String &name, const T& value)
    {
        std::ostringstream oss; oss << value;
        Set(name, String(oss.str()));
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
};


template<>
inline void XMLAttribute::Set(const String &name, const Path& filepath)
{
    Set(name, filepath.GetAbsolute());
}

template<>
inline void XMLAttribute::Set(const String &name, const bool& value)
{
    Set(name, value ? "True" : "False");
}

template<>
inline bool XMLAttribute::Get() const
{
    return GetStringValue().EqualsNoCase("true");
}

template<>
inline String XMLAttribute::Get() const
{
    return GetStringValue();
}

template<>
inline Path XMLAttribute::Get() const
{
    if ( GetStringValue().IsEmpty() ) { return Path::Empty; }
    return Path(GetStringValue());
}

NAMESPACE_BANG_END

#endif // XMLATTRIBUTE_H
