#ifndef TREEHIERARCHY_H
#define TREEHIERARCHY_H

#include "Bang.h"

#include <GL/glew.h>

#include <map>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <QTimer>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QApplication>
#include <QDropEvent>

#include "Stage.h"
#include "Entity.h"
#include "IWindowEventManagerListener.h"

class TreeHierarchy : public QTreeWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    //For every entity, we have the associated treeItem,
    //in order to update :)
    mutable std::map<Entity*, QTreeWidgetItem*> entityToTreeItem;
    mutable std::map<QTreeWidgetItem*,Entity*> treeItemToEntity;

    void ExpandRecursiveUpwards(QTreeWidgetItem *item);
    QTreeWidgetItem* FillRecursiveDownwards(Entity *e);

    //Useful for example, for RemoveChild
    //(we just need to remove the parent/s of all the selected entities)
    void LeaveOnlyTopLevelItems(std::list<QTreeWidgetItem*> *items);

    void UnselectAll();

public:
    explicit TreeHierarchy(QWidget *parent = 0);
    virtual ~TreeHierarchy();

    void FillDownwards(Stage *currentStage);

    Entity* GetFirstSelectedEntity() const;

    void OnChildAdded(Entity *child) override;
    void OnChildChangedParent(Entity *child, Entity *previousParent) override;
    void OnChildRemoved(Entity *child) override;

    void dropEvent(QDropEvent *event);

public slots:

    void OnContextMenuCreateEmptyClicked();
    void OnContextMenuDeleteClicked();
    void OnCustomContextMenuRequested(QPoint point);

    void _NotifyHierarchyItemSelected(QTreeWidgetItem *item, int column);
};

#endif // TREEHIERARCHY_H
