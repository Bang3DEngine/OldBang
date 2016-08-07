#ifndef IDROPPABLE_H
#define IDROPPABLE_H

#include <QWidget>
#include <QListView>
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>

#include "File.h"
#include "GameObject.h"

// T must derive from QWidget
template <class T>
class IDroppableWidget : public T
{
protected:
    IDroppableWidget()
    {
        T::setAcceptDrops(true);
        // T must be subclass of QWidget
        //QWidget *qw = new T(); delete qw;
    }

    virtual void dragEnterEvent(QDragEnterEvent *e)
    {
        T::dragEnterEvent(e);
        e->accept();
    }

    virtual void dragMoveEvent(QDragMoveEvent *e)
    {
        T::dragMoveEvent(e);
        e->accept();
    }

    virtual void dragLeaveEvent(QDragLeaveEvent *e)
    {
        T::dragLeaveEvent(e);
        e->accept();
    }

    virtual void dropEvent(QDropEvent *e)
    {
        T::dropEvent(e);
        e->accept();
    }

    virtual void OnDropFromExplorer(const File &f, QDropEvent *e) {}
    virtual void OnDropFromHierarchy(GameObject *go,  QDropEvent *e) {}
};

// Had to do it like this because of circular dependencies and templates

class IDroppableQWidget : public IDroppableWidget<QWidget>
{
public:
    IDroppableQWidget();
    virtual void dropEvent(QDropEvent *e);
};
class IDroppableQListView : public IDroppableWidget<QListView>
{
public:
    IDroppableQListView();
    virtual void dropEvent(QDropEvent *e);
};
class IDroppableQTreeWidget : public IDroppableWidget<QTreeWidget>
{
public:
    IDroppableQTreeWidget();
    virtual void dropEvent(QDropEvent *e);
};


#endif // IDROPPABLE_H
