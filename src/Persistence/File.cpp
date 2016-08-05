#include "File.h"

#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
#include "Texture2D.h"

File::File(const QFileSystemModel *model, const QModelIndex *index)
{
    m_isFile = !model->isDir(*index);

    m_path = Persistence::ProjectRootAbsoluteToRelative(model->filePath(*index).toStdString());
    m_name = model->fileName(*index).toStdString();

    std::string::size_type p = m_name.find('.', 2);
    if (p != std::string::npos)
    {
        m_extension = m_name.substr(p+1, m_name.length() - p - 1);
        m_name = m_name.substr(0, p);
    }
}

bool File::IsTexture2DAsset() const
{
    return  m_isFile && m_extension == Texture2D::GetFileExtensionStatic();
}

bool File::IsImageFile() const
{
    return  m_isFile && (
             m_extension == "jpg"  ||
             m_extension == "jpeg" ||
             m_extension == "png"  ||
             m_extension == "bmp"
            );
}

bool File::IsMeshAsset() const
{
    return  m_isFile && m_extension == Mesh::GetFileExtensionStatic();
}

bool File::IsMeshFile() const
{
    return  m_isFile && (m_extension == "obj");
}

bool File::IsMaterialAsset() const
{
    return m_isFile && m_extension == Material::GetFileExtensionStatic();
}

bool File::IsTextFile() const
{
    return m_isFile && (
           m_extension == "txt"  ||
           m_extension == "frag" ||
           m_extension == "vert"
          );
}

bool File::IsPrefabAsset() const
{
    return m_isFile && m_extension == Prefab::GetFileExtensionStatic();
}

bool File::IsDir() const
{
    return !m_isFile;
}

bool File::IsFile() const
{
    return m_isFile;
}

const std::string &File::GetName() const
{
    return m_name;
}

const std::string &File::GetRelativePath() const
{
    return m_path;
}

const std::string &File::GetExtension() const
{
    return m_extension;
}

std::string File::GetNameAndExtension() const
{
    std::string str = GetName();
    str += ".";
    str += GetExtension();
    return  str;
}
