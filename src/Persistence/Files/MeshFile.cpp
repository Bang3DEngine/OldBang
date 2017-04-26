#include "Bang/MeshFile.h"

#include "Bang/Debug.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/XMLProperty.h"
#include "Bang/FileInspectable.h"

MeshFile::MeshFile()
{
}

MeshFile::MeshFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    m_numFaces = FileReader::GetMeshNumTriangles(m_path);
}

const QPixmap& MeshFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Data);
}

void MeshFile::Read(const XMLNode &xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void MeshFile::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshFile");

    xmlInfo->SetString("FileName", GetNameAndExtension(), {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", IsTriangles() ? "Triangles" : "Quads",
                      {XMLProperty::Readonly});
    xmlInfo->SetString("Faces",  String(GetNumFaces()),
                      {XMLProperty::Readonly});
}


#ifdef BANG_EDITOR
IInspectable *MeshFile::GetNewInspectable()
{
    return new FileInspectable<MeshFile>(*this);
}
#endif

bool MeshFile::IsAsset() const
{
    return false;
}

bool MeshFile::IsTriangles() const
{
    return m_isTris;
}

int MeshFile::GetNumFaces() const
{
    return m_numFaces;
}
