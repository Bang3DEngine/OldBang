#include "Bang/SerializableObject.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"

SerializableObject::SerializableObject()
{
}

SerializableObject::~SerializableObject()
{
}

XMLNode SerializableObject::GetXMLInfo() const
{
    XMLNode xmlInfo;
    Write(&xmlInfo);
    return xmlInfo;
}

String SerializableObject::GetSerializedString() const
{
    XMLNode xmlInfo;
    Write(&xmlInfo);
    return xmlInfo.ToString();
}

void SerializableObject::ReadFromString(const String &xmlInfoString)
{
    XMLNode xmlInfo = XMLNode::FromString(xmlInfoString);
    Read(xmlInfo);
    PostRead(xmlInfo);
}

void SerializableObject::ReadReflection(const XMLNode &xmlInfo)
{
}

void SerializableObject::WriteReflection(XMLNode *xmlInfo) const
{
}

void SerializableObject::Read(const XMLNode &xmlInfo)
{
    ReadReflection(xmlInfo);
}

void SerializableObject::Write(XMLNode *xmlInfo) const
{
    WriteReflection(xmlInfo);
    xmlInfo->SetTagName( GetClassName() );
}

bool SerializableObject::ReadFromFile(const Path &path)
{
    if (path.Exists())
    {
        String fileContents = File::GetContents(path);
        ReadFromString(fileContents);
        return true;
    }
    return false;
}

bool SerializableObject::WriteToFile(const Path &path) const
{
    File::Write(path, GetSerializedString());
    return true;
}

void SerializableObject::PostRead(const XMLNode &xmlInfo) {}
