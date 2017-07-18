#include "Bang/InspectorWidgetGroup.h"

#include "Bang/InspectorWidget.h"

InspectorWidgetGroup::InspectorWidgetGroup()
{
}

InspectorWidgetGroup::~InspectorWidgetGroup()
{
    for (InspectorWidget *inspWidget : GetInspectorWidgets())
    {
        inspWidget->OnDestroy();
    }
}

void InspectorWidgetGroup::Init()
{
}

void InspectorWidgetGroup::OnUpdate()
{
    for (InspectorWidget *inspWidget : GetInspectorWidgets())
    {
        inspWidget->OnUpdate();
    }
}

String InspectorWidgetGroup::GetTitle() const
{
    return "Title";
}

bool InspectorWidgetGroup::NeedsEnableCheckBox() const
{
    return false;
}

void InspectorWidgetGroup::OnEnableCheckBoxChanged(bool enabled)
{
}

bool InspectorWidgetGroup::IsEnabled() const
{
    return false;
}

void InspectorWidgetGroup::InsertInspectorWidget(
        InspectorWidget *inspectorWidget)
{
    inspectorWidget->Init();
    inspectorWidget->OnUpdate();

    QObject::connect(inspectorWidget, SIGNAL(Changed(IInspectorWidget*)),
                     this, SLOT(OnInspectorWidgetChanged(IInspectorWidget*)));

    inspectorWidget->RegisterDestroyListener(this);
    m_inspectorWidgets.PushBack(inspectorWidget);
    emit RequestInsert(inspectorWidget);
}

void InspectorWidgetGroup::InsertInspectorWidgets(
        const List<InspectorWidget*>& inspectorWidgets)
{
    for (InspectorWidget* inspWidget : inspectorWidgets)
    {
        InsertInspectorWidget(inspWidget);
    }
}

void InspectorWidgetGroup::MoveInspectorWidget(InspectorWidget *inspWidget,
                                               int newIndex)
{
    auto oldIt = m_inspectorWidgets.Find(inspWidget);
    auto newIt = m_inspectorWidgets.Begin();
    std::advance(newIt, newIndex);

    m_inspectorWidgets.InsertAfter(newIt, inspWidget);
    m_inspectorWidgets.Remove(oldIt);

    emit RequestMove(inspWidget, newIndex);
}

List<InspectorWidget*>::Iterator InspectorWidgetGroup::RemoveInspectorWidget(
                                               InspectorWidget *inspWidget)
{
    emit RequestRemove(inspWidget);
    return m_inspectorWidgets.Remove(inspWidget);
}

const List<InspectorWidget*>& InspectorWidgetGroup::GetInspectorWidgets() const
{
    return m_inspectorWidgets;
}

void InspectorWidgetGroup::OnInspectorWidgetChanged(
        IInspectorWidget *changedInspWidget)
{
}

void InspectorWidgetGroup::OnDestroyDemanded(Destroyable *objectDemandingDestroy)
{
    for (auto it = GetInspectorWidgets().Begin();
         it != GetInspectorWidgets().End(); )
    {
        InspectorWidget *inspectorWidget = *it;
        if (inspectorWidget == objectDemandingDestroy)
        {
            it = RemoveInspectorWidget(inspectorWidget);
        }
        else { ++it; }
    }
}
