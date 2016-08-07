#include "File.h"

#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
#include "Texture2D.h"

File::File()
{
}

File::File(const QFileSystemModel *model, const QModelIndex *index)
{
    m_isFile = !model->isDir(*index);

    m_absPath = model->filePath(*index).toStdString();
    m_path = Persistence::ProjectRootAbsoluteToRelative(m_absPath);
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
    return  m_isFile && IsOfExtension(Texture2D::GetFileExtensionStatic());
}

bool File::IsImageFile() const
{
    return  m_isFile && IsOfExtension("jpg jpeg png bmp");
}

bool File::IsMeshAsset() const
{
    return  m_isFile && IsOfExtension(Mesh::GetFileExtensionStatic());
}

bool File::IsMeshFile() const
{
    return  m_isFile && IsOfExtension("obj");
}

bool File::IsMaterialAsset() const
{
    return m_isFile && IsOfExtension(Material::GetFileExtensionStatic());
}

bool File::IsTextFile() const
{
    return m_isFile && IsOfExtension("txt frag vert cpp h");
}

bool File::IsPrefabAsset() const
{
    return m_isFile && IsOfExtension(Prefab::GetFileExtensionStatic());
}

bool File::IsDir() const
{
    return !m_isFile;
}

bool File::IsFile() const
{
    return m_isFile;
}

bool File::IsOfExtension(const std::string &extensions) const
{
    // We receive something like "jpg png bmp obj"
    std::vector<std::string> extensionsList = StringUtils::SplitTrim(extensions, ' ');
    for (std::string ext : extensionsList)
    {
        if (ext == GetExtension())
        {
            return true;
        }
    }
    return false;
}

std::string File::GetContents() const
{
    return FileReader::GetContents(m_absPath);
}

const std::string &File::GetAbsolutePath() const
{
    return m_absPath;
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
