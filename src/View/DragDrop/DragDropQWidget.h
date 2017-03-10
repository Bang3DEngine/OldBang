#ifndef DRAGDROPQWIDGET_H
#define DRAGDROPQWIDGET_H

#include <QWidget>

#include "DragDropAgent.h"

class DragDropQWidget : public QWidget,
                        public DragDropAgent

{
public:
    DragDropQWidget(QWidget *parent = nullptr);
    virtual ~DragDropQWidget();

    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

#endif // DRAGDROPQWIDGET_H
