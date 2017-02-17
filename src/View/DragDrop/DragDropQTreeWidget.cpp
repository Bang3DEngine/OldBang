#include "DragDropQTreeWidget.h"


DragDropQTreeWidget::DragDropQTreeWidget()
    : QTreeWidget(), DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}

DragDropQTreeWidget::~DragDropQTreeWidget()
{
}

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
