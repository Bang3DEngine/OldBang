#include "DragDropAgent.h"

#include "EditorWindow.h"
#include "WindowEventManager.h"

#include "File.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"


// DragDropAgent
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
//

DragDropQWidget::DragDropQWidget() : DragDropAgent()
{
    setAcceptDrops(true);
}
DragDropQWidget::~DragDropQWidget() { }
void DragDropQWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QWidget::dragEnterEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QWidget::dragLeaveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QWidget::dragMoveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQWidget::dropEvent(QDropEvent *e)
{
    QWidget::dropEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}

//

DragDropQListView::DragDropQListView() : DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}
DragDropQListView::~DragDropQListView() { }
void DragDropQListView::dragEnterEvent(QDragEnterEvent *e)
{
    QListView::dragEnterEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListView::dragLeaveEvent(QDragLeaveEvent *e)
{
    QListView::dragLeaveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListView::dragMoveEvent(QDragMoveEvent *e)
{
    QListView::dragMoveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListView::dropEvent(QDropEvent *e)
{
    QListView::dropEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
//

DragDropQTreeWidget::DragDropQTreeWidget() : DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}
DragDropQTreeWidget::~DragDropQTreeWidget() { }
void DragDropQTreeWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QTreeWidget::dragEnterEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQTreeWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QTreeWidget::dragLeaveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQTreeWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QTreeWidget::dragMoveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQTreeWidget::dropEvent(QDropEvent *e)
{
    QTreeWidget::dropEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}

//

DragDropQListWidget::DragDropQListWidget()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}
DragDropQListWidget::~DragDropQListWidget() { }
void DragDropQListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QListWidget::dragEnterEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QListWidget::dragLeaveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QListWidget::dragMoveEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
void DragDropQListWidget::dropEvent(QDropEvent *e)
{
    QListWidget::dropEvent(e);
    if (MustIgnoreThisEvent(e)) { e->ignore(); } else { e->accept(); }
}
