#include "Bang/DragDropQListView.h"

DragDropQListView::DragDropQListView()
    : QListView(), DragDropAgent()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}

DragDropQListView::~DragDropQListView()
{
}

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
