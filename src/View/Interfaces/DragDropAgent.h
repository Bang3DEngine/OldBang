#ifndef IDROPPABLE_H
#define IDROPPABLE_H

#include <QWidget>
#include <QListView>
#include <QListWidget>
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
class EditorWindow;
class WindowEventManager;

/**
 * @brief These objects/widgets will be the ones who can emit drag/drop events.
 */
class DragDropAgent
{
private:
    /**
     * Specifies from which dragDrop sourceObjects this agent should accept
     * dragDrop events from. In other words: if this ddAgent receives a
     * dragDrop event from another dragDropAgent {A}, the event will be accepted
     * if {A} is in this list. The event will be ignored if {A} is NOT in this list.
    **/
    List<QObject*> m_dragDropEventPossibleSources;

protected:
    DragDropAgent() {}
    virtual ~DragDropAgent() {}

    void SetDragDropEventPossibleSources(
            const List<QObject*> &possibleDragDropEventSources);

    bool MustIgnoreThisEvent(QEvent *e) const;
};

//

class DragDropQWidget : public DragDropAgent,
                        public QWidget
{
public:
    DragDropQWidget();
    virtual ~DragDropQWidget();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

//

class DragDropQListView : public DragDropAgent,
                          public QListView
{
public:
    DragDropQListView();
    virtual ~DragDropQListView();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

//

class DragDropQTreeWidget : public DragDropAgent,
                            public QTreeWidget
{
public:
    DragDropQTreeWidget();
    virtual ~DragDropQTreeWidget();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

//

class DragDropQListWidget : public DragDropAgent,
                            public QListWidget
{
public:
    DragDropQListWidget();
    virtual ~DragDropQListWidget();
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

#endif // IDROPPABLE_H
