#ifndef DRAGDROPQLISTVIEW_H
#define DRAGDROPQLISTVIEW_H

#include <QListView>
#include "Bang/WinUndef.h"

#include "Bang/DragDropAgent.h"

class DragDropQListView : public QListView,
                          public DragDropAgent
{
public:
    DragDropQListView();
    virtual ~DragDropQListView();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

#endif // DRAGDROPQLISTVIEW_H
