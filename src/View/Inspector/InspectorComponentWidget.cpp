#include "InspectorComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

InspectorComponentWidget::InspectorComponentWidget(Component *relatedComponent) :
    InspectorWidget(relatedComponent)
{
    this->m_relatedComponent = relatedComponent;

    m_IsEnabledCheckbox = new QCheckBox();
    m_IsEnabledCheckbox->setChecked(this->m_relatedComponent->IsEnabled());
    connect(m_IsEnabledCheckbox, SIGNAL(clicked(bool)),
            this, SLOT(OnEnabledCheckboxPressed(bool)));
    m_titleLayout->addWidget(m_IsEnabledCheckbox, 1);

    m_titleLabel->setText(
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
    if (m_relatedComponent )
    {
        m_relatedComponent->SetEnabled(checked);
    }
}

void InspectorComponentWidget::OnContextMenuRemoveComponentSelected()
{
    m_relatedComponent->gameObject->RemoveComponent(m_relatedComponent);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveUpSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, -1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void InspectorComponentWidget::OnContextMenuMoveDownSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, 1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

