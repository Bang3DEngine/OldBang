#include "MaterialAssetFile.h"

#include "XMLParser.h"
#include "Persistence.h"

#ifdef BANG_EDITOR
#include "MaterialAssetFileInspectable.h"
#endif

MaterialAssetFile::MaterialAssetFile(const QFileSystemModel *model, const QModelIndex &index) :
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
            m_textureFilepaths.PushBack(xmlInfo->GetFilepath("Texture" + std::to_string(i)));
        }

        m_diffuseColor = xmlInfo->GetVector4("DiffuseColor");
        delete xmlInfo;
    }
}

QPixmap MaterialAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Icons/MaterialAssetIcon.png",
                                        Persistence::c_EngineAssetsRootAbsolute);
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *MaterialAssetFile::GetInspectable() const
{
    return new MaterialAssetFileInspectable(*this);
}
#endif

const String &MaterialAssetFile::GetVertexShaderFilepath() const
{
    return m_vshaderFilepath;
}

const String &MaterialAssetFile::GetFragmentShaderFilepath() const
{
    return m_fshaderFilepath;
}

int MaterialAssetFile::GetTextureCount() const
{
    return m_textureCount;
}

const Array<String> &MaterialAssetFile::GetTextureFilepaths() const
{
    return m_textureFilepaths;
}

const Vector4 &MaterialAssetFile::GetDiffuseColor() const
{
    return m_diffuseColor;
}
