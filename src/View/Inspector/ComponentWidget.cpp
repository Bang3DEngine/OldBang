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

    QAction actionCopyComponent("Copy Component", this);
    QAction actionPasteComponent("Paste Component", this);
    QAction actionPasteComponentValues("Paste Component values", this);
    QAction actionMoveComponentUp("Move Component up", this);
    QAction actionMoveComponentDown("Move Component down", this);
    QAction actionRemoveComponent("Remove Component", this);

    connect(&actionMoveComponentUp, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveUpSelected()));
    connect(&actionMoveComponentDown, SIGNAL(triggered()),
            this, SLOT(OnContextMenuMoveDownSelected()));
    connect(&actionCopyComponent, SIGNAL(triggered()),
            this, SLOT(OnContextMenuCopyComponentSelected()));
    connect(&actionPasteComponent, SIGNAL(triggered()),
            this, SLOT(OnContextMenuPasteComponentSelected()));
    connect(&actionPasteComponentValues, SIGNAL(triggered()),
            this, SLOT(OnContextMenuPasteComponentValuesSelected()));
    connect(&actionRemoveComponent, SIGNAL(triggered()),
            this, SLOT(OnContextMenuRemoveComponentSelected()));

    contextMenu.addAction(&actionMoveComponentUp);
    contextMenu.addAction(&actionMoveComponentDown);
    contextMenu.addAction(&actionCopyComponent);
    contextMenu.addAction(&actionPasteComponent);
    contextMenu.addAction(&actionPasteComponentValues);
    contextMenu.addAction(&actionRemoveComponent);

    if (ComponentClipboard::IsCopiedTransform())
    {
        actionPasteComponent.setEnabled(false);
    }

    if (ComponentClipboard::IsEmpty())
    {
        actionPasteComponent.setEnabled(false);
        actionPasteComponentValues.setEnabled(false);
    }

    if (m_relatedComponent->GetName() == "Transform")
    {
        actionRemoveComponent.setEnabled(false);
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

void ComponentWidget::OnContextMenuCopyComponentSelected()
{
    QTimer::singleShot(50, this, SLOT(CopyComponentSelected()));
}
void ComponentWidget::CopyComponentSelected()
{
    ComponentClipboard::CopyComponent(m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidget::OnContextMenuPasteComponentSelected()
{
    QTimer::singleShot(50, this, SLOT(PasteComponentSelected()));
}
void ComponentWidget::PasteComponentSelected()
{
    ComponentClipboard::PasteComponentInto(m_relatedComponent->gameObject);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidget::OnContextMenuPasteComponentValuesSelected()
{
    QTimer::singleShot(50, this, SLOT(PasteComponentValuesSelected()));
}
void ComponentWidget::PasteComponentValuesSelected()
{
    ComponentClipboard::PasteComponentValuesInto(m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidget::OnContextMenuRemoveComponentSelected()
{
    QTimer::singleShot(50, this, SLOT(RemoveComponentSelected()));
}
void ComponentWidget::RemoveComponentSelected()
{
    m_relatedComponent->gameObject->RemoveComponent(m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

