#ifndef DRAGDROPQLISTWIDGET_H
#define DRAGDROPQLISTWIDGET_H

#include <QListWidget>

#include "DragDropAgent.h"

class DragDropQListWidget : public QListWidget,
                            public DragDropAgent
{
public:
    DragDropQListWidget();
    virtual ~DragDropQListWidget();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};
#endif // DRAGDROPQLISTWIDGET_H
