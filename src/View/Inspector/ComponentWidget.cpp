#include "ComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "WindowMain.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget()
{
    Init(relatedComponent->GetName(), relatedComponent);

    m_relatedComponent = relatedComponent;

    if (relatedComponent->GetName() != "Transform")
    {
        m_enabledCheckbox = new QCheckBox();
        m_enabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_gridLayout->addWidget(m_enabledCheckbox, 0, 1, 1, 1,
                                Qt::AlignRight | Qt::AlignVCenter);
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

// We do this with timers to avoid a bug
void ComponentWidget::OnContextMenuRemoveComponentSelected()
{
    QTimer::singleShot(50, this, SLOT(RemoveComponentSelected()));
}
void ComponentWidget::RemoveComponentSelected()
{
    m_relatedComponent->gameObject->RemoveComponent(m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidget::OnContextMenuMoveUpSelected()
{
    QTimer::singleShot(50, this, SLOT(MenuMoveUpSelected()));
}
void ComponentWidget::MenuMoveUpSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, -1);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidget::OnContextMenuMoveDownSelected()
{
    QTimer::singleShot(50, this, SLOT(MenuMoveDownSelected()));
}
void ComponentWidget::MenuMoveDownSelected()
{
    m_relatedComponent->gameObject->MoveComponent(m_relatedComponent, 1);
    Inspector::GetInstance()->RefreshHard();
}

