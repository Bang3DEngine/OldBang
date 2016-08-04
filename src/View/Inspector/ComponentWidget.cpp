#include "ComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(relatedComponent->GetName(), relatedComponent)
{
    m_relatedComponent = relatedComponent;

    if (!dynamic_cast<Transform*>(relatedComponent))
    {
        m_IsEnabledCheckbox = new QCheckBox();
        m_IsEnabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_IsEnabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_titleLayout->addWidget(m_IsEnabledCheckbox, 1);
    }
}

ComponentWidget::~ComponentWidget()
{

}

void ComponentWidget::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Widget comp context menu"), this);

    QAction actionMoveComponentUp("Move up", this);
    QAction actionMoveComponentDown("Move down", this);

    connect(&actionMoveComponentUp, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveUpSelected()));
    connect(&actionMoveComponentDown, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveDownSelected()));

    contextMenu.addAction(&actionMoveComponentUp);
    contextMenu.addAction(&actionMoveComponentDown);

    if (!dynamic_cast<Transform*>(m_relatedComponent))
    {
        QAction actionRemoveComponent("Remove Component", this);
        connect(&actionRemoveComponent, SIGNAL(triggered()),
                this, SLOT(OnContextMenuRemoveComponentSelected()));
        contextMenu.addAction(&actionRemoveComponent);
    }

    contextMenu.exec(mapToGlobal(point));
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    if (m_relatedComponent )
    {
        m_relatedComponent->SetEnabled(checked);
    }
}

void ComponentWidget::OnContextMenuRemoveComponentSelected()
{
    m_relatedComponent->gameObject->RemoveComponent(m_relatedComponent);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void ComponentWidget::OnContextMenuMoveUpSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, -1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

void ComponentWidget::OnContextMenuMoveDownSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, 1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

