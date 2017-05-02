#include "Bang/SerializableObject.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "Bang/IO.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
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

void SerializableObject::Read(const XMLNode &xmlInfo)
{
    XMLParser::RegisterId(&xmlInfo, this);
}

void SerializableObject::Write(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
    xmlInfo->SetTagName( GetClassName() );
}

bool SerializableObject::ReadFromFile(const Path &path)
{
    if (path.Exists())
    {
        String fileContents = IO::GetFileContents(path);
        ReadFromString(fileContents);
        return true;
    }
    return false;
}

bool SerializableObject::WriteToFile(const Path &path) const
{
    return IO::WriteToFile(path, GetSerializedString(true));
}

void SerializableObject::PostRead(const XMLNode &xmlInfo) {}

const QPixmap &SerializableObject::GetIcon() const
{
    return IconManager::GetEmptyPixmap();
}

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
    Inspector::GetInstance()->OnSerializableObjectDestroyed(this);
    #endif
}
