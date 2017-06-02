#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <set>
#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/Object.h"
#include "Bang/String.h"
#include "Bang/IReflectable.h"

class String;
class XMLNode;
class InspectorWidget;
class SerializableObject : public Object,
                           public IReflectable
{
    OBJECT(SerializableObject)

public:
    virtual ~SerializableObject();

    XMLNode GetXMLInfo() const;
    String GetSerializedString(bool writeToFile = true) const;
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

    virtual const QPixmap& GetIcon() const;
    virtual String GetFileExtension() const;

protected:
    SerializableObject();
};

#endif // SERIALIZABLEOBJECT_H
