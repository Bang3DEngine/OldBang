#include "Bang/TransformInspectorWidget.h"

TransformInspectorWidget::TransformInspectorWidget(Transform *transform)
    : ComponentWidget(transform)
{
    p_transform = transform;
}

TransformInspectorWidget::~TransformInspectorWidget()
{
}

void TransformInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_positionAW = new AttrWidgetVectorFloat("Position", 3);
    m_rotationAW = new AttrWidgetVectorFloat("Rotation", 3);
    m_scaleAW    = new AttrWidgetVectorFloat("Scale", 3);

    InsertAttributeWidget(m_positionAW);
    InsertAttributeWidget(m_rotationAW);
    InsertAttributeWidget(m_scaleAW);

    SetEnabledCheckboxVisible(false);
}

void TransformInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();

    m_positionAW->SetValue( p_transform->GetLocalPosition() );
    m_rotationAW->SetValue( p_transform->GetLocalEuler() );
    m_scaleAW->SetValue( p_transform->GetLocalScale() );
}

void TransformInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_positionAW)
    {
        p_transform->SetLocalPosition( m_positionAW->GetVector3() );
    }
    else if (attrWidget == m_rotationAW)
    {
        p_transform->SetLocalEuler( m_rotationAW->GetVector3() );
    }
    else if (attrWidget == m_scaleAW)
    {
        p_transform->SetLocalScale( m_scaleAW->GetVector3() );
    }

    emit Changed(this);
}

void TransformInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
