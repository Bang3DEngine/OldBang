#include "DragDropQWidget.h"

DragDropQWidget::DragDropQWidget()
    : QWidget(), DragDropAgent()
{
    setAcceptDrops(true);
}

DragDropQWidget::~DragDropQWidget()
{
}

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
