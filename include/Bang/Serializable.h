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

class Serializable : public IGUIDable
{
public:
    virtual ~Serializable();
    Serializable(const Serializable &rhs);

    void ImportXML(const String &xmlInfoString);

    virtual void ImportXML(const XMLNode &xmlInfo);
    virtual void ExportXML(XMLNode *xmlInfo) const;

    virtual bool ImportXMLFromFile(const Path &path);
    virtual bool ExportXMLToFile(const Path &path) const;

    virtual String GetClassName() const = 0;

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
