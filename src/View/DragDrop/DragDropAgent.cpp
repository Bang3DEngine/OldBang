#include "Bang/DragDropAgent.h"

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/DragDropManager.h"
#include "Bang/WindowEventManager.h"

void DragDropAgent::SetDragDropEventPossibleSources(
        const List<QObject *> &possibleDragDropEventSources)
{
    m_dragDropEventPossibleSources = possibleDragDropEventSources;
}

bool DragDropAgent::MustIgnoreThisEvent(QEvent *e) const
{
    DragDropInfo *ddi = DragDropManager::GetDragDropInfo();
    return !m_dragDropEventPossibleSources.Contains(ddi->sourceObject);
}
