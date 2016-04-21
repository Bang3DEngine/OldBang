#ifndef HIERARCHY_H
#define HIERARCHY_H

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

class Hierarchy : public QTreeWidget, public IWindowEventManagerListener
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

    Stage *currentStage;

public:
    explicit Hierarchy(QWidget *parent = 0);
    virtual ~Hierarchy();

    void FillDownwards(Stage *currentStage);

    Entity* GetFirstSelectedEntity() const;

    void OnChildAdded(Entity *child) override;
    void OnChildChangedParent(Entity *child, Entity *previousParent) override;
    void OnChildRemoved(Entity *child) override;

    void dropEvent(QDropEvent *event);

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;

    void keyPressEvent(QKeyEvent *e);

public slots:

    void OnItemNameChanged(QTreeWidgetItem *item, int column);

    void OnCustomContextMenuRequested(QPoint point);
    void OnContextMenuCreateEmptyClicked();
    void OnContextMenuDeleteClicked();

    void _NotifyHierarchyItemSelectionChanged();
};

#endif // TREEHIERARCHY_H
