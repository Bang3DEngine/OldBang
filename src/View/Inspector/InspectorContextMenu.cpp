#include "InspectorContextMenu.h"

#include "Inspector.h"
#include "InspectorWidget.h"

InspectorContextMenu::InspectorContextMenu(Inspector *inspector)
    : ContextMenu(inspector), m_inspector(inspector)
{
}

void InspectorContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    if ( m_inspector->m_currentGameObject &&
        !m_inspector->m_currentInspectorWidgets.empty())
    {
        QMenu contextMenu(tr("Inspector context menu"), m_inspector);

        QAction actionPasteComponent("Paste Component", m_inspector);

        ComponentWidget *cw =
                static_cast<ComponentWidget*>(m_inspector->
                                              m_currentInspectorWidgets[0]);

        connect(&actionPasteComponent, SIGNAL(triggered()),
                &(cw->m_cwContextMenu), SLOT(OnPasteComponentSelected()));
        contextMenu.addAction(&actionPasteComponent);

        if (ComponentClipboard::IsEmpty())
        {
            actionPasteComponent.setEnabled(false);
        }

        contextMenu.exec(m_inspector->mapToGlobal(point));
    }
}
