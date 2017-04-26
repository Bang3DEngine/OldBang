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
    : ContextMenu(inspector), p_inspector(inspector)
{
}

void InspectorContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    ENSURE(!p_inspector->GetCurrentInspectorWidgets().Empty());
    InspectorWidget *iw = p_inspector->GetCurrentInspectorWidgets().Front();

    if (Object::IsOfType<ComponentWidget>(iw))
    {
        QMenu contextMenu(tr("Inspector context menu"), p_inspector);
        contextMenu.addActions(EditorWindow::GetInstance()->
                               menuComponents->actions());
        contextMenu.addSeparator();

        ComponentWidget *cw = Object::SCast<ComponentWidget>(iw);
        QAction actionPasteComponent("Paste Component", p_inspector);
        connect(&actionPasteComponent, SIGNAL(triggered()),
                &(cw->m_cwContextMenu), SLOT(OnPasteComponentSelected()));
        contextMenu.addAction(&actionPasteComponent);

        if (ComponentClipboard::IsEmpty())
        {
            actionPasteComponent.setEnabled(false);
        }
        contextMenu.exec(p_inspector->mapToGlobal(point));
    }
}
