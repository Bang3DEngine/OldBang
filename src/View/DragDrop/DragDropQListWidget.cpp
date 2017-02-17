#include "DragDropQListWidget.h"

DragDropQListWidget::DragDropQListWidget()
    : QListWidget(), DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}

DragDropQListWidget::~DragDropQListWidget()
{
}

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
