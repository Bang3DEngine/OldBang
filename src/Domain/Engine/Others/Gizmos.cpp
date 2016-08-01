#include "Gizmos.h"

#include "Box.h"
#include "Vector3.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

EditorGameObject *Gizmos::m_gizmosGameObject = nullptr;

SingleLineRenderer *Gizmos::m_singleLineRenderer = nullptr;
MeshRenderer *Gizmos::m_meshRenderer = nullptr;
Mesh *Gizmos::m_boxMesh = nullptr;

Material *Gizmos::m_material = nullptr;

Vector3 Gizmos::m_color = Vector3::one;

bool Gizmos::m_inited = false;

void Gizmos::Init()
{
    if (!Gizmos::m_inited)
    {
        Gizmos::m_boxMesh = MeshFactory::GetCube();
        Gizmos::m_material = AssetsManager::GetAsset<Material>("./Assets/Engine/Materials/D2G_Default.bmat");

        Gizmos::m_inited = true;
    }
}

void Gizmos::SetGizmosGameObject(EditorGameObject *ego)
{
    Init();

    if (m_gizmosGameObject)
    {
        delete m_gizmosGameObject;
    }
    m_gizmosGameObject = ego;

    Gizmos::m_singleLineRenderer = m_gizmosGameObject->AddComponent<SingleLineRenderer>();
    Gizmos::m_singleLineRenderer->SetMaterial(Gizmos::m_material);
    Gizmos::m_meshRenderer = m_gizmosGameObject->AddComponent<MeshRenderer>();
    Gizmos::m_meshRenderer->SetMaterial(Gizmos::m_material);
}

void Gizmos::SetColor(const Vector3 &color)
{
    m_color = color;
}

void Gizmos::DrawBox(const Box &b, bool wireframe)
{
    Gizmos::Init();

    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_boxMesh);
    Gizmos::m_meshRenderer->GetMaterial()->SetDiffuseColor(glm::vec4(Gizmos::m_color.r,
                                                                     Gizmos::m_color.g,
                                                                     Gizmos::m_color.b, 1));
    Gizmos::m_meshRenderer->SetDrawWireframe(wireframe);
    Gizmos::m_gizmosGameObject->transform->SetPosition(b.GetCenter());
    Gizmos::m_gizmosGameObject->transform->SetScale(b.GetDimensions());
    Gizmos::m_meshRenderer->OnRender();
}
