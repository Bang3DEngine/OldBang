#include "SingleInspectorWidgetGroup.h"

#include "Bang/InspectorWidget.h"

SingleInspectorWidgetGroup::SingleInspectorWidgetGroup(
        InspectorWidget *inspWidget)
{
    InsertInspectorWidget(inspWidget);
    inspWidget->SetHeaderVisible(false);
}
