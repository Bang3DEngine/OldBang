#include "InspectorComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

InspectorComponentWidget::InspectorComponentWidget(Component *relatedComponent) :
    InspectorWidget(relatedComponent)
{
    this->p_relatedComponent = relatedComponent;

    p_IsEnabledCheckbox = new QCheckBox();
    p_IsEnabledCheckbox->setChecked(this->p_relatedComponent->IsEnabled());
    connect(p_IsEnabledCheckbox, SIGNAL(clicked(bool)),
            this, SLOT(OnEnabledCheckboxPressed(bool)));
    p_titleLayout->addWidget(p_IsEnabledCheckbox, 1);

    p_titleLabel->setText(
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
    if(p_relatedComponent )
    {
        p_relatedComponent->SetEnabled(checked);
    }
}

void InspectorComponentWidget::OnContextMenuRemoveComponentSelected()
{
    p_relatedComponent->gameObject->RemoveComponent(p_relatedComponent);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveUpSelected()
{
    p_relatedComponent->gameObject->MoveComponent(p_relatedComponent, -1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveDownSelected()
{
    p_relatedComponent->gameObject->MoveComponent(p_relatedComponent, 1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

