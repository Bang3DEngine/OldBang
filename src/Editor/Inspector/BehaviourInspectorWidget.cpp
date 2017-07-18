#include "BehaviourInspectorWidget.h"

#include "Bang/Behaviour.h"
#include "Bang/Extensions.h"

BehaviourInspectorWidget::BehaviourInspectorWidget(Behaviour *behaviour)
    : ComponentWidget(behaviour)
{
    p_behaviour = behaviour;
}

BehaviourInspectorWidget::~BehaviourInspectorWidget()
{

}

void BehaviourInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_sourceFileAW = new AttrWidgetFile("Source file",
                                        Extensions::GetList<Behaviour>());
    InsertAttributeWidget(m_sourceFileAW);
}

void BehaviourInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();

    m_sourceFileAW->SetValue( p_behaviour->GetSourceFilepath() );
}

void BehaviourInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_sourceFileAW)
    {
        p_behaviour->SetSourceFilepath(m_sourceFileAW->GetPath());
    }

    emit Changed(this);
}

void BehaviourInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
