#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "Bang/String.h"
#include "Bang/HideFlags.h"
#include "Bang/IGUIDable.h"
#include "Bang/ICloneable.h"
#include "Bang/IReflectable.h"

NAMESPACE_BANG_BEGIN

#define SERIALIZABLE(CLASS) \
        public: \
        virtual String GetClassName() const override { return #CLASS; } \
        static String GetClassNameStatic() { return #CLASS; }

class Serializable : public ICloneable,
                     public virtual IGUIDable,
                     public IReflectable
{
public:
    virtual ~Serializable();
    Serializable(const Serializable &rhs);

    void ImportXML(const String &xmlInfoString);

    virtual void ImportXMLReflection(const XMLNode &xmlInfo);
    virtual void ExportXMLReflection(XMLNode *xmlInfo) const;

    virtual void ImportXML(const XMLNode &xmlInfo);
    virtual void ExportXML(XMLNode *xmlInfo) const;

    virtual bool ImportXMLFromFile(const Path &path);
    virtual bool ExportXMLToFile(const Path &path) const;

    virtual void PostImportXML(const XMLNode &xmlInfo);

    virtual void CloneInto(ICloneable*) const override;
    virtual String GetClassName() const = 0;
    virtual String GetInstanceId() const;

    XMLNode GetXMLInfo() const;
    String GetSerializedString() const;

    HideFlags& GetHideFlags();
    const HideFlags& GetHideFlags() const;

protected:
    Serializable();

private:
    HideFlags m_hideFlags;

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // SERIALIZABLE_H
