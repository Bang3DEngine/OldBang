#include "BehaviourInspectorWidget.h"

#include "Bang/Behaviour.h"

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

    m_sourceFileAW = new AttrWidgetFile("Source file");
    InsertAttributeWidget(m_sourceFileAW);
}

void BehaviourInspectorWidget::Refresh()
{
    ComponentWidget::Refresh();

    m_sourceFileAW->SetValue( p_behaviour->GetSourceFilepath() );
}

void BehaviourInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    if (attrWidget == m_sourceFileAW)
    {
        p_behaviour->SetSourceFilepath(m_sourceFileAW->GetPath());
    }
}

void BehaviourInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
