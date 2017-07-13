#include "Bang/MeshRendererInspectorWidget.h"

#include "Bang/Mesh.h"
#include "Bang/AssetsManager.h"

MeshRendererInspectorWidget::MeshRendererInspectorWidget(
        MeshRenderer *meshRenderer)
    : RendererInspectorWidget(meshRenderer)
{
    p_meshRenderer = meshRenderer;
}

MeshRendererInspectorWidget::~MeshRendererInspectorWidget()
{
}

void MeshRendererInspectorWidget::InitExtra()
{
    RendererInspectorWidget::InitExtra();

    m_meshAW = new AttrWidgetFile("Mesh");

    InsertAttributeWidget(m_meshAW);
}

void MeshRendererInspectorWidget::Refresh()
{
    RendererInspectorWidget::Refresh();

    Mesh *mesh = p_meshRenderer->GetMesh();
    Path meshPath = mesh ? mesh->GetFilepath() : Path::Empty;
    m_meshAW->SetValue( meshPath );
}

void MeshRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_meshAW)
    {
        Mesh *mesh = AssetsManager::Load<Mesh>(m_meshAW->GetPath());
        p_meshRenderer->SetMesh( mesh );
    }
}

void MeshRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
