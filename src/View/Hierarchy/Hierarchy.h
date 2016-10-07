#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "Bang.h"

#include <GL/glew.h>

#include "Array.h"

#include <chrono>
#include <algorithm>

#include <QTimer>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QApplication>
#include <QDropEvent>

#include "Map.h"
#include "List.h"
#include "File.h"
#include "Scene.h"
#include "FileDialog.h"
#include "GameObject.h"
#include "DragDropAgent.h"
#include "IShortcutListener.h"
#include "GameObjectClipboard.h"
#include "HierarchyContextMenu.h"
#include "HierarchyDragDropManager.h"
#include "IWindowEventManagerListener.h"

class Hierarchy : public DragDropQTreeWidget,
                  public IWindowEventManagerListener,
                  public IShortcutListener
{
    Q_OBJECT

friend class Screen;
friend class HierarchyContextMenu;
friend class HierarchyDragDropManager;

private:
    //For every gameObject, we have the associated treeItem,
    //in order to update :)
    mutable Map<GameObject*, QTreeWidgetItem*> m_gameObject_To_TreeItem;
    mutable Map<QTreeWidgetItem*,GameObject*> m_treeItem_To_GameObject;

    QTimer m_refreshTimer;
    HierarchyContextMenu m_hContextMenu;
    HierarchyDragDropManager m_hDragDropManager;

    void Expand(QTreeWidgetItem *item);
    QTreeWidgetItem* PopulateItemGameObject(GameObject *e);

    //Useful for example, for Removing a Child
    //(we just need to remove the parent/s of all the selected entities)
    void LeaveOnlyOuterMostItems(List<QTreeWidgetItem*> *items);

    QTreeWidgetItem* GetFirstSelectedItem() const;
    GameObject *GetGameObjectFromItem(QTreeWidgetItem *item) const;
    QTreeWidgetItem *GetItemFromGameObject(GameObject *go) const;

public:
    explicit Hierarchy(QWidget *parent = 0);
    virtual ~Hierarchy();

    void Clear();
    void Expand(GameObject *go);
    bool IsSelected(GameObject *go);
    bool IsSelected(QTreeWidgetItem *item);
    void ExpandToggle(GameObject *go);

    GameObject* GetFirstSelectedGameObject() const;

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;

    void OnShortcutPressed() override;

    List<GameObject*> GetSelectedGameObjects(bool excludeInternal = false);
    void SelectGameObject(GameObject *go);
    void SelectItemAboveOrBelowSelected(bool above);
    void UnselectAll();

    virtual void dropEvent(QDropEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

    void OnGameObjectNameChanged(GameObject *go);

    static Hierarchy *GetInstance();

    bool Contains(GameObject *go);
    void DeleteGameObjectItem(GameObject *go);

private slots:
    void RefreshFromScene();
    void UpdateSceneFromHierarchy();
    void UpdateGameObjectFromHierarchy(QTreeWidgetItem *goItem);
    QTreeWidgetItem* Refresh(GameObject *go);

public slots:
    void OnGameObjectDestroyed(GameObject *destroyed) override;
    void OnItemNameChanged(QTreeWidgetItem *item, int column);

    void OnSelectionChanged();
    void _NotifyHierarchyGameObjectSelectionChanged();
    void _NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // TREEHIERARCHY_H
