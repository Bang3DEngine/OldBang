#include "InspectorComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

InspectorComponentWidget::InspectorComponentWidget(Component *relatedComponent) :
    InspectorWidget(relatedComponent)
{
    this->relatedComponent = relatedComponent;

    IsEnabledCheckbox = new QCheckBox();
    IsEnabledCheckbox->setChecked(this->relatedComponent->IsEnabled());
    connect(IsEnabledCheckbox, SIGNAL(clicked(bool)),
            this, SLOT(OnEnabledCheckboxPressed(bool)));
    titleLayout->addWidget(IsEnabledCheckbox, 1);

    titleLabel->setText(
                QString::fromStdString(relatedComponent->GetName())
                );
}

InspectorComponentWidget::~InspectorComponentWidget()
{

}

void InspectorComponentWidget::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Widget comp context menu"), this);

    QAction actionMoveComponentUp("Move up", this);
    QAction actionMoveComponentDown("Move down", this);
    QAction actionRemoveComponent("Remove Component", this);

    connect(&actionMoveComponentUp, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveUpSelected()));
    connect(&actionMoveComponentDown, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveDownSelected()));
    connect(&actionRemoveComponent, SIGNAL(triggered()),
            this, SLOT(OnContextMenuRemoveComponentSelected()));

    contextMenu.addAction(&actionMoveComponentUp);
    contextMenu.addAction(&actionMoveComponentDown);
    contextMenu.addAction(&actionRemoveComponent);

    contextMenu.exec(mapToGlobal(point));
}

void InspectorComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    if(relatedComponent )
    {
        relatedComponent->SetEnabled(checked);
    }
}

void InspectorComponentWidget::OnContextMenuRemoveComponentSelected()
{
    relatedComponent->gameObject->RemoveComponent(relatedComponent);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveUpSelected()
{
    relatedComponent->gameObject->MoveComponent(relatedComponent, -1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveDownSelected()
{
    relatedComponent->gameObject->MoveComponent(relatedComponent, 1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

