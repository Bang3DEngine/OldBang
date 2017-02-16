#include "IFileable.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "XMLParser.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "Persistence.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

String IFileable::GetXMLInfoString() const
{
    XMLNode *xmlInfo = new XMLNode();
    FillXMLInfo(xmlInfo);
    String result = xmlInfo->ToString();
    delete xmlInfo;
    return result;
}

void IFileable::ReadXMLInfoFromString(const String &xmlInfoString)
{
    XMLNode *xmlInfo = XMLNode::FromString(xmlInfoString);
    if (xmlInfo)
    {
        ReadXMLInfo(xmlInfo);
        delete xmlInfo;
    }
}

void IFileable::ReadXMLInfo(const XMLNode *xmlInfo)
{
    XMLParser::RegisterId(xmlInfo, this);
}

void IFileable::FillXMLInfo(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
}

void IFileable::PostReadXMLInfo(const XMLNode *xmlInfo) {}
void IFileable::PostFillXMLInfo(XMLNode *xmlInfo) const {}

void IFileable::OnFileNameChanged(const String &absFilepathBefore,
                                  const String &absFilepathNow)
{
    String xmlInfoStr = GetXMLInfoString();

    String relPathBefore = Persistence::ToRelative(absFilepathBefore, false);
    String relPathNow    = Persistence::ToRelative(absFilepathNow, false);
    int replacements = xmlInfoStr.Replace(relPathBefore, relPathNow);

    if (replacements > 0)
    {
        XMLNode *newXMLInfo = XMLNode::FromString(xmlInfoStr);
        ReadXMLInfo(newXMLInfo);
        delete newXMLInfo;
    }
}

IFileable::IFileable()
{
    #ifdef BANG_EDITOR
    Explorer *exp = Explorer::GetInstance();
    if (exp) { exp->m_fileables.insert(this); }
    #endif
}

IFileable::~IFileable()
{
    #ifdef BANG_EDITOR
    Explorer *exp = Explorer::GetInstance();
    if (exp) { exp->m_fileables.erase(this); }
    #endif
}

#ifdef BANG_EDITOR
void IFileable::OnFileNameChangedStatic(const String &absFilepathBefore,
                                        const String &absFilepathNow)
{
    Explorer *exp = Explorer::GetInstance();
    for (IFileable *fileable : exp->m_fileables)
    {
        fileable->OnFileNameChanged(absFilepathBefore, absFilepathNow);
    }
}
#endif
