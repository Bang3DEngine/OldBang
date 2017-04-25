#include "ShaderProgramAssetFile.h"

#include "Bang/IO.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/ShaderProgramAssetFileInspectable.h"

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

#ifdef BANG_EDITOR
IInspectable *ShaderProgramAssetFile::GetNewInspectable() const
{
    return nullptr;//new ShaderProgramAssetFileInspectable(*this);
}
#endif
