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

#include "Scene.h"
#include "FileDialog.h"
#include "GameObject.h"
#include "IWindowEventManagerListener.h"

class Hierarchy : public QTreeWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    //For every gameObject, we have the associated treeItem,
    //in order to update :)
    mutable std::map<GameObject*, QTreeWidgetItem*> gameObjectToTreeItem;
    mutable std::map<QTreeWidgetItem*,GameObject*> treeItemToGameObject;

    void ExpandRecursiveUpwards(QTreeWidgetItem *item);
    QTreeWidgetItem* FillDownwards(GameObject *e);

    //Useful for example, for RemoveChild
    //(we just need to remove the parent/s of all the selected entities)
    void LeaveOnlyTopLevelItems(std::list<QTreeWidgetItem*> *items);

    Scene *currentScene = nullptr;

public:
    explicit Hierarchy(QWidget *parent = 0);
    virtual ~Hierarchy();

    void Refresh(Scene *currentScene);

    GameObject* GetFirstSelectedGameObject() const;

    void OnChildAdded(GameObject *child) override;
    void OnChildChangedParent(GameObject *child, GameObject *previousParent) override;
    void OnChildRemoved(GameObject *child) override;

    void dropEvent(QDropEvent *event);

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;

    void keyPressEvent(QKeyEvent *e);

    void SelectGameObject(GameObject *go);
    void UnselectAll();

public slots:

    void OnItemNameChanged(QTreeWidgetItem *item, int column);

    void OnCustomContextMenuRequested(QPoint point);
    void OnContextMenuCreateEmptyClicked();
    void OnContextMenuDeleteClicked();
    void OnContextMenuCreatePrefab();

    void OnSelectionChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void _NotifyHierarchyGameObjectSelectionChanged();
    void _NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // TREEHIERARCHY_H
