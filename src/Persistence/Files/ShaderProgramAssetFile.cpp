#include "ShaderProgramAssetFile.h"

#include "Bang/IO.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

ShaderProgramAssetFile::ShaderProgramAssetFile()
{
}

ShaderProgramAssetFile::ShaderProgramAssetFile(const Path& path)
    : File(path)
{
}

const QPixmap &ShaderProgramAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/BehaviourIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

Path ShaderProgramAssetFile::GetVertexShaderFilepath() const
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath().GetAbsolute());
    return xmlInfo.GetFilepath("VertexShader");
}

Path ShaderProgramAssetFile::GetFragmentShaderFilepath() const
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath().GetAbsolute());
    return xmlInfo.GetFilepath("FragmentShader");
}

void ShaderProgramAssetFile::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    IO::WriteToFile(GetPath().GetAbsolute(), xmlInfo.ToString());
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
}

void ShaderProgramAssetFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

    xmlInfo->SetFilepath("VertexShader", GetVertexShaderFilepath(),
                         "vert glsl");
    xmlInfo->SetFilepath("FragmentShader", GetFragmentShaderFilepath(),
                         "frag glsl");
}

#ifdef BANG_EDITOR
IInspectable *ShaderProgramAssetFile::GetNewInspectable()
{
    return new FileInspectable<ShaderProgramAssetFile>(*this);
}
#endif
