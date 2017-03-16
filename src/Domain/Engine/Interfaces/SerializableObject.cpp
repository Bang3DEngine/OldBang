#include "SerializableObject.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "XMLParser.h"
#include "FileReader.h"
#include "IO.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#include "FileReferencesManager.h"
#endif

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
    XMLNode *xmlInfo = XMLNode::FromString(xmlInfoString);
    if (xmlInfo)
    {
        Read(*xmlInfo);
        PostRead(*xmlInfo);
        delete xmlInfo;
    }
}

void SerializableObject::Read(const XMLNode &xmlInfo)
{
    XMLParser::RegisterId(&xmlInfo, this);
}

void SerializableObject::Write(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
}

bool SerializableObject::ReadFromFile(const String &absPath)
{
    if (IO::Exists(absPath))
    {
        String fileContents = IO::GetFileContents(absPath);
        ReadFromString(fileContents);
        return true;
    }
    return false;
}

bool SerializableObject::WriteToFile(const String &absPath) const
{
    return IO::WriteToFile(absPath, GetSerializedString());
}

void SerializableObject::PostRead(const XMLNode &xmlInfo) {}

String SerializableObject::GetFileExtension() const
{
    return "";
}

SerializableObject::SerializableObject()
{
    #ifdef BANG_EDITOR
    FileReferencesManager *frm = FileReferencesManager::GetInstance();
    if (frm) { frm->RegisterSerializableObject(this); }
    #endif
}

SerializableObject::~SerializableObject()
{
    #ifdef BANG_EDITOR
    FileReferencesManager *frm = FileReferencesManager::GetInstance();
    if (frm) { frm->UnRegisterSerializableObject(this); }
    #endif
}
