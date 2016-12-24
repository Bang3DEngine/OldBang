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
class EditorWindow;
class WindowEventManager;

/**
 * @brief These objects/widgets will be the ones who can emit drag/drop events.
 */
class DragDropAgent
{
protected:
    DragDropAgent() {}
    virtual ~DragDropAgent() {}
};

//

class DragDropQWidget : public DragDropAgent,
                        public QWidget
{
public:
    DragDropQWidget();
    virtual ~DragDropQWidget();
};

//

class DragDropQListView : public DragDropAgent,
                          public QListView
{
public:
    DragDropQListView();
    virtual ~DragDropQListView();
};

//

class DragDropQTreeWidget : public DragDropAgent,
                            public QTreeWidget
{
public:
    DragDropQTreeWidget();
    virtual ~DragDropQTreeWidget();
};

#endif // IDROPPABLE_H
