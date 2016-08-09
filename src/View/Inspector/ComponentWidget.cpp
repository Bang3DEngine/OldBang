#include "ComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(relatedComponent->GetName(),
                    relatedComponent)
{
    m_relatedComponent = relatedComponent;

    if (relatedComponent->GetName() != "Transform")
    {
        m_enabledCheckbox = new QCheckBox();
        m_enabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_titleLayout->addWidget(m_enabledCheckbox, 1);
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
    QAction actionRemoveComponent("Remove Component", this);

    connect(&actionMoveComponentUp, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveUpSelected()));
    connect(&actionMoveComponentDown, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveDownSelected()));

    contextMenu.addAction(&actionMoveComponentUp);
    contextMenu.addAction(&actionMoveComponentDown);

    if (m_relatedComponent->GetName() != "Transform")
    {
        connect(&actionRemoveComponent, SIGNAL(triggered()),
                this, SLOT(OnContextMenuRemoveComponentSelected()));
        contextMenu.addAction(&actionRemoveComponent);
        Logger_Log("hola");
    }
    contextMenu.exec(mapToGlobal(point));
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    if (m_relatedComponent)
    {
        m_relatedComponent->SetEnabled(checked);
    }
}

void ComponentWidget::OnContextMenuRemoveComponentSelected()
{
    m_relatedComponent->gameObject->RemoveComponent(m_relatedComponent);
    Inspector::GetInstance()->Refresh();
}

void ComponentWidget::OnContextMenuMoveUpSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, -1);
    Inspector::GetInstance()->Refresh();
}

void ComponentWidget::OnContextMenuMoveDownSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, 1);
    WindowMain::GetInstance()->widgetInspector->Refresh();
}

