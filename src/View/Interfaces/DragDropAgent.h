#ifndef IDROPPABLE_H
#define IDROPPABLE_H

#include <QWidget>
#include <QListView>
#include <QTreeWidget>
#include <QMainWindow>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>

#include "Color.h"
#include "Vector2.h"
#include "DragDropManager.h"

class File;
class GameObject;
class WindowMain;
class WindowEventManager;

/**
 * @brief These objects/widgets will be the ones who can emit drag/drop events.
 */
class DragDropAgent
{
protected:
    DragDropAgent() {}
    virtual ~DragDropAgent() {}

    void HandleDragEnterEvent(QDragEnterEvent *e);
    void HandleDragMoveEvent(QDragMoveEvent *e);
    void HandleDragLeaveEvent(QDragLeaveEvent *e);
    void HandleDropEvent(QDropEvent *e);
};

//

class DragDropQWidget : public DragDropAgent,
                        public QWidget
{
public:
    DragDropQWidget();
    virtual ~DragDropQWidget();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

//

class DragDropQListView : public DragDropAgent,
                          public QListView
{
public:
    DragDropQListView();
    virtual ~DragDropQListView();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

//

class DragDropQTreeWidget : public DragDropAgent,
                            public QTreeWidget
{
public:
    DragDropQTreeWidget();
    virtual ~DragDropQTreeWidget();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

#endif // IDROPPABLE_H
