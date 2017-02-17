#include "DragDropAgent.h"

#include "EditorWindow.h"
#include "WindowEventManager.h"

#include "File.h"
#include "Debug.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"

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
