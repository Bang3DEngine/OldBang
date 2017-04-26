#include "ShaderProgramAssetFile.h"

#include "Bang/IO.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

ShaderProgramAssetFile::ShaderProgramAssetFile()
{
}

ShaderProgramAssetFile::ShaderProgramAssetFile(const QFileSystemModel *model,
                                               const QModelIndex &index)
    : File(model, index)
{
}

const QPixmap &ShaderProgramAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/BehaviourIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

String ShaderProgramAssetFile::GetVertexShaderFilepath() const
{
    XMLNode *xmlInfo = XMLParser::FromFile(GetAbsolutePath());
    if (!xmlInfo) { return ""; }

    String vShaderFilepath = xmlInfo->GetString("VertexShader");
    delete xmlInfo;

    return vShaderFilepath;
}

String ShaderProgramAssetFile::GetFragmentShaderFilepath() const
{
    XMLNode *xmlInfo = XMLParser::FromFile(GetAbsolutePath());
    if (!xmlInfo) { return ""; }

    String fShaderFilepath = xmlInfo->GetString("FragmentShader");
    delete xmlInfo;

    return fShaderFilepath;
}

void ShaderProgramAssetFile::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    IO::WriteToFile(GetAbsolutePath(), xmlInfo.ToString());
    AssetsManager::UpdateAsset(GetAbsolutePath(), xmlInfo);
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
