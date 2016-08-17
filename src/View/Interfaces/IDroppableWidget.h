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

class File;
class GameObject;
class WindowMain;
class WindowEventManager;
class IDroppable
{
private:
    bool m_inited = false;

protected:

    IDroppable();
    virtual ~IDroppable();

    void Init(QWidget *qw);

    void OnDragEnterEvent(QWidget *qw, QDragEnterEvent *e);
    void OnDragMoveEvent(QWidget *qw, QDragMoveEvent *e);
    void OnDragLeaveEvent(QWidget *qw, QDragLeaveEvent *e);
    void OnDropEvent(QWidget *qw, QDropEvent *e);

    virtual void OnDropFromExplorer(const File &f, QDropEvent *e) {}
    virtual void OnDropFromHierarchy(GameObject *go,  QDropEvent *e) {}

public:
    static QString acceptDragStyle, acceptDragStyleOnlyBorder,
                   overDragStyle, rejectDragStyle;

};

class IDroppableQWidget : public IDroppable,
                          public QWidget
{
public:
    IDroppableQWidget();
    virtual ~IDroppableQWidget();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};
class IDroppableQListView : public IDroppable,
                            public QListView
{
public:
    IDroppableQListView();
    virtual ~IDroppableQListView();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};
class IDroppableQTreeWidget : public IDroppable,
                              public QTreeWidget
{
public:
    IDroppableQTreeWidget();
    virtual ~IDroppableQTreeWidget();
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

#endif // IDROPPABLE_H
