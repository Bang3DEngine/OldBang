#include "DragDropAgent.h"

#include "EditorWindow.h"
#include "WindowEventManager.h"

#include "File.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"


// DragDropAgent
void DragDropAgent::HandleDragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
    DragDropManager::HandleDragEnterEvent(dynamic_cast<QObject*>(this), e);
}

void DragDropAgent::HandleDragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
    DragDropManager::HandleDragMoveEvent(dynamic_cast<QObject*>(this), e);
}

void DragDropAgent::HandleDragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
    DragDropManager::HandleDragLeaveEvent(dynamic_cast<QObject*>(this), e);
}

void DragDropAgent::HandleDropEvent(QDropEvent *e)
{
    e->accept();
    DragDropManager::HandleDropEvent(dynamic_cast<QObject*>(this), e);
}
//


// CTORS
DragDropQWidget::DragDropQWidget() : DragDropAgent()
{
    setAcceptDrops(true);
}

DragDropQListView::DragDropQListView() : DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}


DragDropQTreeWidget::DragDropQTreeWidget() : DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);

}

DragDropQWidget::~DragDropQWidget()
{
}
DragDropQListView::~DragDropQListView()
{
}
DragDropQTreeWidget::~DragDropQTreeWidget()
{
}
//


// DRAG ENTER
void DragDropQWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QWidget::dragEnterEvent(e);
    DragDropAgent::HandleDragEnterEvent(e);
}
void DragDropQListView::dragEnterEvent(QDragEnterEvent *e)
{
    QListView::dragEnterEvent(e);
    DragDropAgent::HandleDragEnterEvent(e);
}
void DragDropQTreeWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QTreeWidget::dragEnterEvent(e);
    DragDropAgent::HandleDragEnterEvent(e);
}
//


// DRAG MOVE
void DragDropQWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QWidget::dragMoveEvent(e);
    DragDropAgent::HandleDragMoveEvent(e);
}
void DragDropQListView::dragMoveEvent(QDragMoveEvent *e)
{
    QListView::dragMoveEvent(e);
    DragDropAgent::HandleDragMoveEvent(e);
}
void DragDropQTreeWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QTreeWidget::dragMoveEvent(e);
    DragDropAgent::HandleDragMoveEvent(e);
}
//


// DRAG LEAVE
void DragDropQWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QWidget::dragLeaveEvent(e);
    DragDropAgent::HandleDragLeaveEvent(e);
}
void DragDropQListView::dragLeaveEvent(QDragLeaveEvent *e)
{
    QListView::dragLeaveEvent(e);
    DragDropAgent::HandleDragLeaveEvent(e);
}
void DragDropQTreeWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QTreeWidget::dragLeaveEvent(e);
    DragDropAgent::HandleDragLeaveEvent(e);
}
//


// DROP
void DragDropQWidget::dropEvent(QDropEvent *e)
{
    QWidget::dropEvent(e);
    DragDropAgent::HandleDropEvent(e);
}
void DragDropQListView::dropEvent(QDropEvent *e)
{
    QListView::dropEvent(e);
    DragDropAgent::HandleDropEvent(e);
}
void DragDropQTreeWidget::dropEvent(QDropEvent *e)
{
    QTreeWidget::dropEvent(e);
    DragDropAgent::HandleDropEvent(e);
}
//
