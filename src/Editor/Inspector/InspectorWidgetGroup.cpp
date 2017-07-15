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
    InsertInspectorWidgets( {inspectorWidget} );
}

void InspectorWidgetGroup::InsertInspectorWidgets(
        const List<InspectorWidget*>& inspectorWidgets)
{
    for (InspectorWidget* inspWidget : inspectorWidgets)
    {
        inspWidget->Init();
        inspWidget->OnUpdate();

        QObject::connect(inspWidget, SIGNAL(Changed(IInspectorWidget*)),
                         this, SLOT(OnInspectorWidgetChanged(IInspectorWidget*)));

        m_inspectorWidgets.PushBack(inspWidget);
    }
}

const List<InspectorWidget*>& InspectorWidgetGroup::GetInspectorWidgets() const
{
    return m_inspectorWidgets;
}

void InspectorWidgetGroup::OnInspectorWidgetChanged(
        IInspectorWidget *changedInspWidget)
{
}

