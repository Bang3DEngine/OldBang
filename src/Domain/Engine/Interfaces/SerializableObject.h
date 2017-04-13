#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <set>
#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/String.h"

class String;
class XMLNode;
class SerializableObject
{
public:
    virtual ~SerializableObject();

    XMLNode GetXMLInfo() const;
    String GetSerializedString(bool writeToFile = true) const;
    void ReadFromString(const String &xmlInfoString);

    virtual void Read(const XMLNode &xmlInfo);
    virtual void Write(XMLNode *xmlInfo) const;

    virtual bool ReadFromFile(const String &absPath);
    virtual bool WriteToFile(const String &absPath) const;

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
