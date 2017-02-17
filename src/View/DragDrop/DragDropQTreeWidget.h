#ifndef DRAGDROPQTREEWIDGET_H
#define DRAGDROPQTREEWIDGET_H

#include <QTreeWidget>

#include "DragDropAgent.h"

class DragDropQTreeWidget : public QTreeWidget,
                            public DragDropAgent
{
public:
    DragDropQTreeWidget();
    virtual ~DragDropQTreeWidget();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};


#endif // DRAGDROPQTREEWIDGET_H
