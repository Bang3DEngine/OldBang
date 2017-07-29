#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include "Bang/Object.h"
#include "Bang/IReflectable.h"

FORWARD class Path;
FORWARD class String;
FORWARD class XMLNode;
FORWARD class InspectorWidget;

class SerializableObject : public Object,
                           public IReflectable
{
    OBJECT(SerializableObject)

public:
    virtual ~SerializableObject();

    XMLNode GetXMLInfo() const;
    String GetSerializedString() const;
    void ReadFromString(const String &xmlInfoString);

    virtual void ReadReflection(const XMLNode &xmlInfo);
    virtual void WriteReflection(XMLNode *xmlInfo) const;

    virtual void Read(const XMLNode &xmlInfo);
    virtual void Write(XMLNode *xmlInfo) const;

    virtual bool ReadFromFile(const Path &path);
    virtual bool WriteToFile(const Path &path) const;

    /**
      This Post stuff is basically for the SerializableObject that need info
      contained in other xmlNodes in the file.
      For example, a Scene might need to access to its main Camera. If the
      Camera hasnt been read yet, then Scene wont be able to set it.
     */
    virtual void PostRead(const XMLNode &xmlInfo);

protected:
    SerializableObject();
};

#endif // SERIALIZABLEOBJECT_H
