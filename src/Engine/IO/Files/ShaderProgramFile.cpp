#include "ShaderProgramFile.h"

#include "Bang/Paths.h"
#include "Bang/XMLParser.h"
#include "Bang/AssetsManager.h"

ShaderProgramFile::ShaderProgramFile()
{
}

ShaderProgramFile::ShaderProgramFile(const Path& path)
    : BFile(path)
{
}

Path ShaderProgramFile::GetVertexShaderFilepath() const
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath());
    return xmlInfo.GetFilepath("VertexShader");
}

Path ShaderProgramFile::GetFragmentShaderFilepath() const
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath());
    return xmlInfo.GetFilepath("FragmentShader");
}

void ShaderProgramFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
    File::Write(GetPath(), xmlInfo.ToString());
}

void ShaderProgramFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    xmlInfo->SetFilepath("VertexShader", GetVertexShaderFilepath());
    xmlInfo->SetFilepath("FragmentShader", GetFragmentShaderFilepath());
}
