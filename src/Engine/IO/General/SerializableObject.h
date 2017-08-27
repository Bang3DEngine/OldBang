#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include "Bang/GUID.h"
#include "Bang/String.h"
#include "Bang/HideFlags.h"
#include "Bang/ICloneable.h"
#include "Bang/IReflectable.h"

#define SERIALIZABLE_OBJECT(CLASS) \
        ICLONEABLE(CLASS)\
        public: \
        virtual String GetClassName() const override { return #CLASS; } \
        static String GetClassNameStatic() { return #CLASS; }

class SerializableObject : public ICloneable,
                           public IReflectable
{
public:
    virtual ~SerializableObject();
    SerializableObject(const SerializableObject &rhs);

    XMLNode GetXMLInfo() const;
    String GetSerializedString() const;
    void ReadFromString(const String &xmlInfoString);

    virtual void ReadReflection(const XMLNode &xmlInfo);
    virtual void WriteReflection(XMLNode *xmlInfo) const;

    virtual void Read(const XMLNode &xmlInfo);
    virtual void Write(XMLNode *xmlInfo) const;

    virtual bool ReadFromFile(const Path &path);
    virtual bool WriteToFile(const Path &path) const;

    virtual void PostRead(const XMLNode &xmlInfo);

    virtual void CloneInto(ICloneable*) const override;
    virtual String GetClassName() const = 0;
    virtual String GetInstanceId() const;

    HideFlags& GetHideFlags();
    const HideFlags& GetHideFlags() const;
    const GUID& GetGUID() const;

protected:
    SerializableObject();

private:
    GUID m_GUID;
    HideFlags m_hideFlags;

    void SetGUID(const GUID &guid);

    friend class Resources;
};

#endif // SERIALIZABLEOBJECT_H
