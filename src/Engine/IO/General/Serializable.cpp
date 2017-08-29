#include "Bang/Serializable.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/GUIDManager.h"

Serializable::Serializable()
{
    SetGUID( GUIDManager::GetNewGUID() );
}

Serializable::~Serializable()
{
}

Serializable::Serializable(const Serializable &rhs)
{
    // Don't copy GUID, intentionally left in blank
}

XMLNode Serializable::GetXMLInfo() const
{
    XMLNode xmlInfo;
    ExportXML(&xmlInfo);
    return xmlInfo;
}

String Serializable::GetSerializedString() const
{
    XMLNode xmlInfo;
    ExportXML(&xmlInfo);
    return xmlInfo.ToString();
}

void Serializable::ImportXML(const String &xmlInfoString)
{
    XMLNode xmlInfo = XMLNode::FromString(xmlInfoString);
    ImportXML(xmlInfo);
    PostImportXML(xmlInfo);
}

void Serializable::ImportXMLReflection(const XMLNode &xmlInfo)
{
}

void Serializable::ExportXMLReflection(XMLNode *xmlInfo) const
{
}

void Serializable::ImportXML(const XMLNode &xmlInfo)
{
    ImportXMLReflection(xmlInfo);
    if (xmlInfo.Contains("GUID"))
    { SetGUID(xmlInfo.Get<GUID>("GUID")); }
}

void Serializable::ExportXML(XMLNode *xmlInfo) const
{
    ExportXMLReflection(xmlInfo);
    xmlInfo->SetTagName( GetClassName() );
    xmlInfo->Set<GUID>( "GUID", GetGUID() );
}

bool Serializable::ImportXMLFromFile(const Path &path)
{
    if (path.Exists())
    {
        String fileContents = File::GetContents(path);
        ImportXML(fileContents);
        return true;
    }
    return false;
}

bool Serializable::ExportXMLToFile(const Path &path) const
{
    File::Write(path, GetSerializedString());
    return true;
}

void Serializable::PostImportXML(const XMLNode &xmlInfo) {}

void Serializable::CloneInto(ICloneable *) const
{
}

String Serializable::GetInstanceId() const
{
    return String::ToString( static_cast<const void*>(this) );
}

HideFlags &Serializable::GetHideFlags() { return m_hideFlags; }
const HideFlags &Serializable::GetHideFlags() const { return m_hideFlags; }

void Serializable::SetGUID(const GUID &guid)
{
    m_GUID = guid;
    GUIDManager::RemoveGUID( GetGUID() );
}

const GUID &Serializable::GetGUID() const
{
    return m_GUID;
}
