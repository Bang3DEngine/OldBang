#include "Bang/MeshInspectorWidget.h"

MeshInspectorWidget::MeshInspectorWidget(const MeshFile &meshFile)
    : AssetInspectorWidget(meshFile)
{
}

MeshInspectorWidget::~MeshInspectorWidget()
{

}
void MeshInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void MeshInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();

    m_modelFileAW = new AttrWidgetFile("Model");

    InsertAttributeWidget(m_modelFileAW);
}

void MeshInspectorWidget::OnUpdate()
{
    AssetInspectorWidget::OnUpdate();

    Mesh *mesh = GetAsset();
    m_modelFileAW->SetValue( mesh->GetModelFilepath() );
}

void MeshInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    Mesh *mesh = GetAsset();
    if (attrWidget == m_modelFileAW)
    {
        mesh->LoadFromFile( m_modelFileAW->GetPath() );
    }

    emit Changed(this);
}
