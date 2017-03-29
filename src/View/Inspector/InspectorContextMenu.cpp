#include "Bang/InspectorContextMenu.h"

#include <QMenu>
#include <QAction>
#include "Bang/WinUndef.h"

#include "Bang/Inspector.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/ComponentWidget.h"
#include "Bang/ComponentClipboard.h"

InspectorContextMenu::InspectorContextMenu(Inspector *inspector)
    : ContextMenu(inspector), m_inspector(inspector)
{
}

void InspectorContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    ASSERT(m_inspector);
    ASSERT(m_inspector->p_currentGameObject);
    ASSERT(!m_inspector->m_currentInspectorWidgets.Empty());

    QMenu contextMenu(tr("Inspector context menu"), m_inspector);
    contextMenu.addActions(EditorWindow::GetInstance()->menuComponents->actions());
    contextMenu.addSeparator();

    ComponentWidget *cw = Object::SCast<ComponentWidget>(
                m_inspector->m_currentInspectorWidgets.Front());

    QAction actionPasteComponent("Paste Component", m_inspector);
    connect(&actionPasteComponent, SIGNAL(triggered()),
            &(cw->m_cwContextMenu), SLOT(OnPasteComponentSelected()));
    contextMenu.addAction(&actionPasteComponent);

    if (ComponentClipboard::IsEmpty())
    {
        actionPasteComponent.setEnabled(false);
    }

    contextMenu.exec(m_inspector->mapToGlobal(point));
}
