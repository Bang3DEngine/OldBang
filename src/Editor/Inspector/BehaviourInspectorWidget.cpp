#include "BehaviourInspectorWidget.h"

#include "Bang/Behaviour.h"

BehaviourInspectorWidget::BehaviourInspectorWidget(Behaviour *behaviour)
    : ComponentWidget(behaviour)
{

}

BehaviourInspectorWidget::~BehaviourInspectorWidget()
{

}
void BehaviourInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}


void BehaviourInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();
}

void BehaviourInspectorWidget::Refresh()
{
    ComponentWidget::Refresh();
}
