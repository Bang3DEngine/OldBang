#include "InspectorPartWidget.h"

#include "Part.h"
#include "Entity.h"
#include "WindowMain.h"

InspectorPartWidget::InspectorPartWidget(Part *relatedPart) : InspectorWidget(relatedPart)
{
    this->relatedPart = relatedPart;

    enabledCheckbox = new QCheckBox();
    enabledCheckbox->setChecked(this->relatedPart->IsEnabled());
    connect(enabledCheckbox, SIGNAL(clicked(bool)), this, SLOT(OnEnabledCheckboxPressed(bool)));
    titleLayout->addWidget(enabledCheckbox, 1);
}

InspectorPartWidget::~InspectorPartWidget()
{

}

void InspectorPartWidget::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Widget part context menu"), this);

    QAction actionRemovePart("Remove Part", this);
    QAction actionMovePartUp("Move up", this);
    QAction actionMovePartDown("Move down", this);

    connect(&actionRemovePart, SIGNAL(triggered()), this, SLOT(OnContextMenuRemovePartSelected()));
    connect(&actionMovePartUp, SIGNAL(triggered()), this, SLOT(OnContextMenuMoveUpSelected()));
    connect(&actionMovePartDown, SIGNAL(triggered()), this, SLOT(OnContextMenuMoveDownSelected()));

    contextMenu.addAction(&actionRemovePart);
    contextMenu.addAction(&actionMovePartUp);
    contextMenu.addAction(&actionMovePartDown);

    contextMenu.exec(mapToGlobal(point));
}

void InspectorPartWidget::OnEnabledCheckboxPressed(bool checked)
{
    if(relatedPart != nullptr)
    {
        relatedPart->SetEnabled(checked);
    }
}

void InspectorPartWidget::OnContextMenuRemovePartSelected()
{
    relatedPart->GetOwner()->RemovePart(relatedPart);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorPartWidget::OnContextMenuMoveUpSelected()
{
}

void InspectorPartWidget::OnContextMenuMoveDownSelected()
{

}

