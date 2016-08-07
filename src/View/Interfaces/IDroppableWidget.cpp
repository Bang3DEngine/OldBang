#include "IDroppableWidget.h"

#include "Explorer.h"
#include "Hierarchy.h"

IDroppableQWidget::IDroppableQWidget() : IDroppableWidget()
{
}

void IDroppableQWidget::dropEvent(QDropEvent *e)
{
    QWidget::dropEvent(e);
    Explorer *explorer   = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer && explorer->IsSelectedAFile())
    {
        OnDropFromExplorer(explorer->GetSelectedFile(), e);
    }
    else if (e->source() == hierarchy)
    {
        OnDropFromHierarchy(hierarchy->GetFirstSelectedGameObject(), e);
    }
}

/////////////////////

IDroppableQListView::IDroppableQListView() : IDroppableWidget()
{
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}

void IDroppableQListView::dropEvent(QDropEvent *e)
{
    QListView::dropEvent(e);
    Explorer *explorer   = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer && explorer->IsSelectedAFile())
    {
        OnDropFromExplorer(explorer->GetSelectedFile(), e);
    }
    else if (e->source() == hierarchy)
    {
        OnDropFromHierarchy(hierarchy->GetFirstSelectedGameObject(), e);
    }
}

//////////////////

IDroppableQTreeWidget::IDroppableQTreeWidget() : IDroppableWidget()
{
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}

void IDroppableQTreeWidget::dropEvent(QDropEvent *e)
{
    QTreeWidget::dropEvent(e);
    Explorer *explorer   = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer && explorer->IsSelectedAFile())
    {
        OnDropFromExplorer(explorer->GetSelectedFile(), e);
    }
    else if (e->source() == hierarchy)
    {
        OnDropFromHierarchy(hierarchy->GetFirstSelectedGameObject(), e);
    }
}
