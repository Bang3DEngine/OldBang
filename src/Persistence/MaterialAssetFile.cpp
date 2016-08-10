#include "MaterialAssetFile.h"

#include "MaterialAssetFileInspectable.h"

MaterialAssetFile::MaterialAssetFile(const QFileSystemModel *model, const QModelIndex *index) :
    File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_vshaderFilepath = xmlInfo->GetString("VertexShader");
        m_fshaderFilepath = xmlInfo->GetString("FragmentShader");

        m_textureCount = xmlInfo->GetInt("TextureCount");
        for (int i = 1; i <= m_textureCount; ++i)
        {
            m_textureFilepaths.push_back(xmlInfo->GetFilepath("Texture" + std::to_string(i)));
        }

        m_diffuseColor = xmlInfo->GetVector4("DiffuseColor");
        delete xmlInfo;
    }
}

QPixmap MaterialAssetFile::GetIcon() const
{
    std::string fp = Persistence::ToAbsolute("./Assets/Engine/Icons/MaterialAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *MaterialAssetFile::GetInspectable() const
{
    return new MaterialAssetFileInspectable(*this);
}

const std::string &MaterialAssetFile::GetVertexShaderFilepath() const
{
    return m_vshaderFilepath;
}

const std::string &MaterialAssetFile::GetFragmentShaderFilepath() const
{
    return m_fshaderFilepath;
}

int MaterialAssetFile::GetTextureCount() const
{
    return m_textureCount;
}

const std::vector<std::string> &MaterialAssetFile::GetTextureFilepaths() const
{
    return m_textureFilepaths;
}

const Vector4 &MaterialAssetFile::GetDiffuseColor() const
{
    return m_diffuseColor;
}
