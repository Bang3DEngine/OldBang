#include "Bang/SerializableObject.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "Bang/File.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"

#ifdef BANG_EDITOR
#include "Bang/Inspector.h"
#include "Bang/InspectorWidget.h"
#include "Bang/FileReferencesManager.h"
#endif

XMLNode SerializableObject::GetXMLInfo() const
{
    XMLNode xmlInfo;
    Write(&xmlInfo);
    return xmlInfo;
}

String SerializableObject::GetSerializedString(bool writeToFile) const
{
    XMLNode xmlInfo;
    Write(&xmlInfo);
    return xmlInfo.ToString(writeToFile);
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
    File::Write(path, GetSerializedString(true));
    return true;
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
    Inspector *insp = Inspector::GetInstance();
    if (insp) { insp->OnSerializableObjectDestroyed(this); }
    #endif
}
