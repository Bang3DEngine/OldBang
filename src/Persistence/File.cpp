#include "File.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
#include "Texture2D.h"

#include "TextFile.h"
#include "MeshFile.h"
#include "ImageFile.h"
#include "MeshAssetFile.h"
#include "MaterialAssetFile.h"
#include "Texture2DAssetFile.h"
#include "TextFileInspectable.h"
#include "MeshAssetFileInspectable.h"
#include "PrefabAssetFileInspectable.h"
#include "MaterialAssetFileInspectable.h"

#include "MeshFileInspectable.h"
#include "ImageFileInspectable.h"
#include "Texture2DAssetFileInspectable.h"

QPixmap File::AddNoAssetFileQPixmapOnTopOf(const QPixmap &pm)
{
    /*
    String fp = Persistence::ToAbsolute("./Assets/Engine/Icons/NoAssetIcon.png");
    QPixmap noAssetPixmap(QString::fromStdString(fp));

    QPixmap result(pm.scaled(128, 128, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
    QPainter painter;
    painter.Begin(&result);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, 128, 128, noAssetPixmap);
    painter.End();
    */
    return pm; //result;
}

File::File()
{
}

File::File(const String &filepath)
    : File(Explorer::GetInstance()->GetFileSystemModel(),
           Explorer::GetInstance()->GetModelIndexFromFilepath(filepath))
{
}

File::File(const QFileSystemModel *model, const QModelIndex &index) :
    m_fileSystemModel(model), m_modelIndex(index)
{
    m_isFile = !model->isDir(index);

    m_absPath = model->filePath(index).toStdString();
    m_path = Persistence::ToRelative(m_absPath);
    m_name = model->fileName(index).toStdString();

    String::size_type p = m_name.find('.', 2);
    if (p != String::npos)
    {
        m_extension = m_name.substr(p+1, m_name.Length() - p - 1);
        m_name = m_name.substr(0, p);
    }
}

File::~File()
{
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

bool File::IsBehaviour() const
{
    return m_isFile && IsOfExtension("cpp hpp c h");
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

bool File::IsOfExtension(const String &extensions) const
{
    // We receive something like "jpg png bmp obj"
    std::vector<String> extensionsList = StringUtils::SplitTrim(extensions, ' ');
    for (String ext : extensionsList)
    {
        if (ext == GetExtension())
        {
            return true;
        }
    }
    return false;
}

File *File::GetSpecificFile(const File &f)
{
    if (!f.IsFile()) return nullptr;

    if (f.IsTexture2DAsset())
    {
        return new Texture2DAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsImageFile())
    {
        return new ImageFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMaterialAsset())
    {
        return new MaterialAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMeshAsset())
    {
        return new MeshAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMeshFile())
    {
        return new MeshFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsPrefabAsset())
    {
        return new File(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsTextFile())
    {
        return new TextFile(f.m_fileSystemModel, f.m_modelIndex);
    }

    return new File(f.m_fileSystemModel, f.m_modelIndex);
}

String File::GetContents() const
{
    return FileReader::GetContents(m_absPath);
}

QPixmap File::GetIcon() const
{
    String fp = "";
    if (IsPrefabAsset())
    {
        fp = Persistence::ToAbsolute("./Assets/Engine/Icons/PrefabAssetIcon.png");
    }
    else if (IsBehaviour())
    {
        fp = Persistence::ToAbsolute("./Assets/Engine/Icons/BehaviourIcon.png");
    }
    else
    {
        fp = Persistence::ToAbsolute("./Assets/Engine/Icons/OtherFileIcon.png");
    }

    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *File::GetInspectable() const
{
    return nullptr;
}

const String &File::GetAbsolutePath() const
{
    return m_absPath;
}

const String &File::GetName() const
{
    return m_name;
}

const String &File::GetRelativePath() const
{
    return m_path;
}

const String &File::GetExtension() const
{
    return m_extension;
}

String File::GetNameAndExtension() const
{
    String str = GetName();
    str += ".";
    str += GetExtension();
    return  str;
}
