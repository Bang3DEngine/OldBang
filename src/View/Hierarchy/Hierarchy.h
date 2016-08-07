#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "Bang.h"

#include <GL/glew.h>

#include <map>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

#include <QTimer>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QApplication>
#include <QDropEvent>

#include "Scene.h"
#include "FileDialog.h"
#include "GameObject.h"
#include "IDroppableWidget.h"
#include "ClipboardGameObject.h"
#include "IWindowEventManagerListener.h"

class Hierarchy : public IDroppableQTreeWidget,
                  public IWindowEventManagerListener
{
    Q_OBJECT

private:
    //For every gameObject, we have the associated treeItem,
    //in order to update :)
    mutable std::map<GameObject*, QTreeWidgetItem*> m_gameObjectToTreeItem;
    mutable std::map<QTreeWidgetItem*,GameObject*> m_treeItemToGameObject;

    void ExpandRecursiveUpwards(QTreeWidgetItem *item);
    QTreeWidgetItem* FillDownwards(GameObject *e);

    //Useful for example, for Removing a Child
    //(we just need to remove the parent/s of all the selected entities)
    void LeaveOnlyOuterMostItems(std::list<QTreeWidgetItem*> *items);
    GameObject *GetGameObjectFromItem(QTreeWidgetItem *item) const;
    QTreeWidgetItem *GetItemFromGameObject(GameObject *go) const;

    GameObject* GetDropTargetGameObject(QDropEvent *e) const;

public:
    explicit Hierarchy(QWidget *parent = 0);
    virtual ~Hierarchy();

    void Refresh();

    GameObject* GetFirstSelectedGameObject() const;

    void OnChildAdded(GameObject *child) override;
    void OnChildChangedParent(GameObject *child, GameObject *previousParent) override;
    void OnChildRemoved(GameObject *child) override;

    void dropEvent(QDropEvent *e) override;
    void OnDropFromExplorer(const File &f, QDropEvent *e) override;

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;

    void keyPressEvent(QKeyEvent *e);

    std::list<GameObject*> GetSelectedGameObjects(bool excludeInternal = false);
    void SelectGameObject(GameObject *go);
    void UnselectAll();

    void OnGameObjectNameChanged(GameObject *go);

    static Hierarchy *GetInstance();

public slots:

    void OnItemNameChanged(QTreeWidgetItem *item, int column);

    void OnCustomContextMenuRequested(QPoint point);
    void OnContextMenuCreateEmptyClicked();
    void OnContextMenuCopyClicked();
    void OnContextMenuPasteClicked();
    void OnContextMenuDuplicateClicked();
    void OnContextMenuCreatePrefab();
    void OnContextMenuDeleteClicked();

    void OnSelectionChanged();
    void _NotifyHierarchyGameObjectSelectionChanged();
    void _NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // TREEHIERARCHY_H
