#include "Bang/ModelFile.h"

#include "Bang/Debug.h"

#include "Bang/Paths.h"
#include "Bang/Path.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/XMLProperty.h"

ModelFile::ModelFile()
{
}

ModelFile::ModelFile(const Path& path)
    : BFile(path)
{
    m_numFaces = FileReader::GetModelNumTriangles( GetPath() );
}

const QPixmap& ModelFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/MeshIcon.png"),
                                   IconManager::IconOverlay::Data);
}

void ModelFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
}

void ModelFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    xmlInfo->SetString("FileName", GetPath().GetNameExt());
    xmlInfo->SetString("Mode", IsTriangles() ? "Triangles" : "Quads");
    xmlInfo->SetString("Faces",  String(GetNumFaces()));
}

bool ModelFile::IsAsset() const
{
    return false;
}

bool ModelFile::IsTriangles() const
{
    return m_isTris;
}

int ModelFile::GetNumFaces() const
{
    return m_numFaces;
}
