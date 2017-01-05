#include "ComponentWidgetContextMenu.h"

#include "Inspector.h"
#include "Component.h"
#include "GameObject.h"
#include "ComponentWidget.h"
#include "ComponentClipboard.h"

ComponentWidgetContextMenu::
    ComponentWidgetContextMenu(ComponentWidget *componentWidget) :

    ContextMenu(componentWidget), m_cWidget(componentWidget)
{
}

void ComponentWidgetContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Widget comp context menu"), m_cWidget);

    QAction        actionCopyComponent("Copy Component",         m_cWidget);
    QAction       actionPasteComponent("Paste Component",        m_cWidget);
    QAction actionPasteComponentValues("Paste Component values", m_cWidget);
    QAction      actionMoveComponentUp("Move Component up",      m_cWidget);
    QAction    actionMoveComponentDown("Move Component down",    m_cWidget);
    QAction      actionRemoveComponent("Remove Component",       m_cWidget);

    connect(&actionMoveComponentUp, SIGNAL(triggered()),
            this, SLOT(OnMoveUpSelected()));
    connect(&actionMoveComponentDown, SIGNAL(triggered()),
            this, SLOT(OnMoveDownSelected()));
    connect(&actionCopyComponent, SIGNAL(triggered()),
            this, SLOT(OnCopyComponentSelected()));
    connect(&actionPasteComponent, SIGNAL(triggered()),
            this, SLOT(OnPasteComponentSelected()));
    connect(&actionPasteComponentValues, SIGNAL(triggered()),
            this, SLOT(OnPasteComponentValuesSelected()));
    connect(&actionRemoveComponent, SIGNAL(triggered()),
            this, SLOT(OnRemoveComponentSelected()));

    contextMenu.addAction(&actionMoveComponentUp);
    contextMenu.addAction(&actionMoveComponentDown);
    contextMenu.addAction(&actionCopyComponent);
    contextMenu.addAction(&actionPasteComponent);
    contextMenu.addAction(&actionPasteComponentValues);
    contextMenu.addAction(&actionRemoveComponent);

    if (ComponentClipboard::GetCopiedComponentName() !=
            m_cWidget->m_relatedComponent->GetName())
    {
        actionPasteComponentValues.setEnabled(false);
    }

    if (ComponentClipboard::IsCopiedTransform())
    {
        actionPasteComponent.setEnabled(false);
    }

    if (ComponentClipboard::IsEmpty())
    {
        actionPasteComponent.setEnabled(false);
        actionPasteComponentValues.setEnabled(false);
    }

    if (m_cWidget->m_relatedComponent->GetName() == "Transform")
    {
        actionRemoveComponent.setEnabled(false);
    }

    contextMenu.exec(m_cWidget->mapToGlobal(point));
}

void ComponentWidgetContextMenu::OnMoveUpSelected()
{
    MenuMoveUpSelected();
}
void ComponentWidgetContextMenu::MenuMoveUpSelected()
{
    m_cWidget->m_relatedComponent->gameObject->
            MoveComponent(m_cWidget->m_relatedComponent, -1);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidgetContextMenu::OnMoveDownSelected()
{
    MenuMoveDownSelected();
}
void ComponentWidgetContextMenu::MenuMoveDownSelected()
{
    m_cWidget->m_relatedComponent->gameObject->
            MoveComponent(m_cWidget->m_relatedComponent, 1);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidgetContextMenu::OnCopyComponentSelected()
{
    CopyComponentSelected();
}
void ComponentWidgetContextMenu::CopyComponentSelected()
{
    ComponentClipboard::CopyComponent(
                m_cWidget->m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidgetContextMenu::OnPasteComponentSelected()
{
    PasteComponentSelected();
}
void ComponentWidgetContextMenu::PasteComponentSelected()
{
    ComponentClipboard::PasteComponentInto(
                m_cWidget->m_relatedComponent->gameObject);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidgetContextMenu::OnPasteComponentValuesSelected()
{
    PasteComponentValuesSelected();
}
void ComponentWidgetContextMenu::PasteComponentValuesSelected()
{
    ComponentClipboard::PasteComponentValuesInto(
                m_cWidget->m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}

void ComponentWidgetContextMenu::OnRemoveComponentSelected()
{
    RemoveComponentSelected();
}
void ComponentWidgetContextMenu::RemoveComponentSelected()
{
    m_cWidget->m_relatedComponent->gameObject->
            RemoveComponent(m_cWidget->m_relatedComponent);
    Inspector::GetInstance()->RefreshHard();
}
