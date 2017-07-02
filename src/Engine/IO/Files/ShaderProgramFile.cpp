#include "ShaderProgramFile.h"

#include "Bang/Paths.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

ShaderProgramFile::ShaderProgramFile()
{
}

ShaderProgramFile::ShaderProgramFile(const Path& path)
    : BFile(path)
{
}

const QPixmap &ShaderProgramFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/BehaviourIcon.png"),
                                   IconManager::IconOverlay::Asset);
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

    #ifdef BANG_EDITOR
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
    #endif
}

void ShaderProgramFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    xmlInfo->SetFilepath("VertexShader", GetVertexShaderFilepath(),
                         "vert glsl");
    xmlInfo->SetFilepath("FragmentShader", GetFragmentShaderFilepath(),
                         "frag glsl");
}

#ifdef BANG_EDITOR
IInspectable *ShaderProgramFile::GetNewInspectable()
{
    return new FileInspectable<ShaderProgramFile>(*this);
}
#endif
