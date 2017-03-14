#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <set>

//Describes something than can be written and read to/from a file
class String;
class XMLNode;
class SerializableObject
{
public:
    XMLNode GetXMLInfo() const;
    String GetSerializedString() const;
    void ReadFromString(const String &xmlInfoString);

    virtual void Read(const XMLNode *xmlInfo);
    virtual void Write(XMLNode *xmlInfo) const;

    virtual bool ReadFromFile(const String &absPath);
    virtual bool WriteToFile(const String &absPath) const;

    /**
      This Post stuff is basically for the SerializableObject that need info
      contained in other xmlNodes in the file.
      For example, a Scene might need to access to its main Camera. If the
      Camera hasnt been read yet, then Scene wont be able to set it.
     */
    virtual void PostRead(const XMLNode *xmlInfo);

protected:
    SerializableObject();
    virtual ~SerializableObject();
};

#endif // SERIALIZABLEOBJECT_H
