#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <GL/glew.h>

#include "Bang/Array.h"

#include <QTimer>
#include <QTreeWidget>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Shortcut.h"
#include "Bang/DragDropQTreeWidget.h"
#include "Bang/HierarchyContextMenu.h"
#include "Bang/HierarchyDragDropManager.h"
#include "Bang/IWindowEventManagerListener.h"

class GameObject;
class Hierarchy : public DragDropQTreeWidget,
                  public IWindowEventManagerListener
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = 0);
    virtual ~Hierarchy();

    void OnWindowShown();

    void Clear();
    void Expand(GameObject *go);
    bool IsSelected(GameObject *go);
    bool IsSelected(QTreeWidgetItem *item);
    void ExpandToggle(GameObject *go);

    GameObject* GetFirstSelectedGameObject() const;

    void OnMenuBarCreateEmptyClicked();

    List<GameObject*> GetSelectedGameObjects(bool excludeInternal = false);
    void SelectGameObject(GameObject *go);
    void SelectItemAboveOrBelowSelected(bool above);
    void UnselectAll();

    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

    void OnGameObjectNameChanged(GameObject *go);

    static Hierarchy *GetInstance();

    bool Contains(GameObject *go);
    void DeleteGameObjectItem(GameObject *go);
    void Print(QTreeWidgetItem *item = nullptr, const String &indent = "");

    void dropEvent(QDropEvent *e) override;

    QSize sizeHint() const override;

public slots:
    void OnGameObjectDestroyed(GameObject *destroyed) override;
    void OnItemNameChanged(QTreeWidgetItem *item, int column);

    void OnUpClicked();
    void OnDownClicked();
    void OnRightClicked();
    void OnLeftClicked();
    void OnToggleClicked();
    void OnRenameClicked();
    void OnCopyClicked();
    void OnPasteClicked();
    void OnDuplicateClicked();
    void OnDeleteClicked();
    void OnSelectionChanged();
    void _NotifyHierarchyGameObjectSelectionChanged();
    void _NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item,
                                                 int column);

private slots:
    void SyncHierarchyFromScene();
    void SyncSceneFromHierarchy();
    void SyncGameObjectFromHierarchy(QTreeWidgetItem *goItem);
    void LocateGameObject(GameObject *gameObjectToLocate,
                          GameObject **gameObjectParent,
                          GameObject **gameObjectAbove,
                          GameObject **gameObjectBelow);
    QTreeWidgetItem* SyncHierarchyFromGameObject(GameObject *go);

private:
    //For every gameObject, we have the associated treeItem,
    //in order to update :)
    mutable Map<GameObject*, QTreeWidgetItem*> m_gameObject_To_TreeItem;
    mutable Map<QTreeWidgetItem*,GameObject*> m_treeItem_To_GameObject;

    LocalShortcut m_upShortcut, m_downShortcut, m_rightShortcut, m_leftShortcut;
    LocalShortcut m_renameShortcut, m_toggleShortcut, m_toggleShortcut2;
    LocalShortcut m_copyShortcut, m_pasteShortcut,
                  m_duplicateShortcut, m_deleteShortcut;

    QTimer m_syncHierarchyFromSceneTimer;
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

    friend class Screen;
    friend class HierarchyContextMenu;
    friend class HierarchyDragDropManager;
};

#endif // TREEHIERARCHY_H
