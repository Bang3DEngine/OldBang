#include "Bang/MeshFileInspectable.h"

#include "Bang/Mesh.h"
#include "Bang/XMLNode.h"

MeshFileInspectable::MeshFileInspectable()
{
}

MeshFileInspectable::MeshFileInspectable(const MeshFile &fileMesh) :
    m_fileMesh(fileMesh)
{
}

const QPixmap &MeshFileInspectable::GetIcon() const
{
    return Mesh::GetIconStatic();
}

void MeshFileInspectable::Read(const XMLNode &xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void MeshFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshFileInspectable");

    xmlInfo->SetString("FileName", m_fileMesh.GetNameAndExtension(), {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", m_fileMesh.IsTriangles() ? "Triangles" : "Quads",
                      {XMLProperty::Readonly});
    xmlInfo->SetString("Faces",  std::to_string(m_fileMesh.GetNumFaces()),
                      {XMLProperty::Readonly});
}
