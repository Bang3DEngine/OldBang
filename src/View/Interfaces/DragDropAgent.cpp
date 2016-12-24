#include "DragDropAgent.h"

#include "EditorWindow.h"
#include "WindowEventManager.h"

#include "File.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"


// DragDropAgent

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
