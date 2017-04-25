#include "Bang/Hierarchy.h"

#include <chrono>
#include <algorithm>

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GameObject.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorCamera.h"
#include "Bang/WindowEventManager.h"

Hierarchy::Hierarchy(QWidget *parent)
 :m_upShortcut       (this, KSeq(Qt::Key_Up),     SLOT(OnUpClicked())),
  m_downShortcut     (this, KSeq(Qt::Key_Down),   SLOT(OnDownClicked())),
  m_rightShortcut    (this, KSeq(Qt::Key_Right),  SLOT(OnRightClicked())),
  m_leftShortcut     (this, KSeq(Qt::Key_Left),   SLOT(OnLeftClicked())),
  m_renameShortcut   (this, KSeq(Qt::Key_F2),     SLOT(OnRenameClicked())),
  m_toggleShortcut   (this, KSeq(Qt::Key_Return), SLOT(OnEnterClicked())),
  m_toggleShortcut2  (this, KSeq(Qt::Key_Space),  SLOT(OnSpaceClicked())),
  m_copyShortcut     (this, KSeq("Ctrl+C"),       SLOT(OnCopyClicked())),
  m_pasteShortcut    (this, KSeq("Ctrl+V"),       SLOT(OnPasteClicked())),
  m_duplicateShortcut(this, KSeq("Ctrl+D"),       SLOT(OnDuplicateClicked())),
  m_deleteShortcut   (this, KSeq("Del"),          SLOT(OnDeleteClicked())),
  m_hContextMenu(this),
  m_hDragDropManager(this)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(itemSelectionChanged()), SLOT(OnSelectionChanged()));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
           SLOT(_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem*,int)));
    connect(&m_syncHierarchyFromSceneTimer, SIGNAL(timeout()),
            this, SLOT(SyncHierarchyFromScene()));
    m_syncHierarchyFromSceneTimer.start(1000);

    setMinimumWidth(150); // For initial size, modify SizeHint
}

Hierarchy::~Hierarchy()
{
    clear();
}

void Hierarchy::OnWindowShown()
{
    this->SetDragDropEventPossibleSources({
          Explorer::GetInstance(), this});
}

void Hierarchy::Expand(QTreeWidgetItem *item)
{
    ENSURE(item);
    item->setExpanded(true);
    Expand(item->parent());
}

void Hierarchy::LeaveOnlyOuterMostItems(List<QTreeWidgetItem*> *items)
{
    // For each item, it will be a top level item,
    // if none of the selected items is its parent
    List<QTreeWidgetItem*> result;
    for (QTreeWidgetItem *item : *items)
    {
        QTreeWidgetItem *parent = item->parent();
        if (!items->Contains(parent))
        {
            result.PushBack(item);
        }
    }
    *items = result;
}

QTreeWidgetItem *Hierarchy::GetFirstSelectedItem() const
{
    if (!selectedItems().empty())
    {
        return selectedItems()[0];
    }
    return nullptr;
}

GameObject *Hierarchy::GetGameObjectFromItem(QTreeWidgetItem *item) const
{
	if (!item) { return nullptr; }
    if (m_treeItem_To_GameObject.ContainsKey(item))
    {
        return m_treeItem_To_GameObject[item];
    }
    return nullptr;
}

QTreeWidgetItem *Hierarchy::GetItemFromGameObject(GameObject *go) const
{
	if (!go) { return nullptr; }
    if (m_gameObject_To_TreeItem.ContainsKey(go))
    {
        QTreeWidgetItem *item = m_gameObject_To_TreeItem[go];
        return item;
    }
    return nullptr;
}

void Hierarchy::UnselectAll()
{
    clearSelection();
    _NotifyHierarchyGameObjectSelectionChanged();
}

void Hierarchy::keyPressEvent(QKeyEvent *e)
{
    if (static_cast<Input::Key>(e->key()) == Input::Key::F)
    {
        GameObject *selected = GetFirstSelectedGameObject();
        if (selected)
        {
            Scene *scene = SceneManager::GetActiveScene();
            EditorScene *edScene = Object::SCast<EditorScene>(scene);
            EditorCamera *edCam = edScene->GetEditorCamera();
            edCam->StartLookAt(selected);
        }
    }

    DragDropQTreeWidget::keyPressEvent(e);
}

void Hierarchy::mousePressEvent(QMouseEvent *e)
{
    QTreeWidget::mousePressEvent(e);
    if (itemAt(e->pos()) == nullptr)
    {
        UnselectAll();
    }
}

void Hierarchy::OnGameObjectNameChanged(GameObject *go)
{
    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        item->setText(0, go->name.ToQString());
    }
}

Hierarchy *Hierarchy::GetInstance()
{
    return EditorWindow::GetInstance()->widgetHierarchy;
}

bool Hierarchy::Contains(GameObject *go)
{
    return GetItemFromGameObject(go) != nullptr;
}


void Hierarchy::Clear()
{
    m_gameObject_To_TreeItem.Clear();
    m_treeItem_To_GameObject.Clear();
    clear();
}

void Hierarchy::SyncHierarchyFromScene()
{
    Scene *scene = SceneManager::GetActiveScene(); ENSURE(scene);

    // Refresh go's children. If we find a new child, add it to topLevel.
    const List<GameObject*> &sceneChildren = scene->GetChildren();
    for (GameObject* child : sceneChildren)
    {
        QTreeWidgetItem *childItem = GetItemFromGameObject(child);
        if (!childItem)
        {
            // New child item !!!
            childItem = SyncHierarchyFromGameObject(child);
            addTopLevelItem(childItem);
            m_gameObject_To_TreeItem[child] = childItem;
            m_treeItem_To_GameObject[childItem] = child;
        }
        else
        {
            SyncHierarchyFromGameObject(child); // Just refresh
        }
    }
}

void Hierarchy::SyncSceneFromHierarchy()
{
    // First case, direct children of scene (top level items)
    Scene *scene = SceneManager::GetActiveScene();
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *childItem = topLevelItem(i);
        SyncGameObjectFromHierarchy(childItem);

        GameObject *childItemGo = GetGameObjectFromItem(childItem);
        childItemGo->SetParent(scene, true, nullptr);
    }
}

void Hierarchy::SyncGameObjectFromHierarchy(QTreeWidgetItem *goItem)
{
    // Iterate all items, and update
    GameObject *go = GetGameObjectFromItem(goItem);
    for (int i = 0; i < goItem->childCount(); ++i)
    {
        QTreeWidgetItem *childItem = goItem->child(i);
        SyncGameObjectFromHierarchy(childItem); // Recursive call

        GameObject *childItemGo = GetGameObjectFromItem(childItem);
        childItemGo->SetParent(go, true, nullptr);
    }
}

void Hierarchy::LocateGameObject(GameObject *gameObjectToLocate,
                                 GameObject **gameObjectParent,
                                 GameObject **gameObjectAbove,
                                 GameObject **gameObjectBelow)
{
    QTreeWidgetItem *item = GetItemFromGameObject(gameObjectToLocate);

    if (gameObjectParent)
    {
        QTreeWidgetItem *parentItem = item->parent();
        *gameObjectParent = parentItem ? GetGameObjectFromItem(parentItem) :
                                         SceneManager::GetActiveScene();
    }
    if (gameObjectAbove)
    {
        QTreeWidgetItem *aboveItem = itemAbove(item);
        *gameObjectAbove = GetGameObjectFromItem(aboveItem);
    }
    if (gameObjectBelow)
    {
        QTreeWidgetItem *belowItem = itemBelow(item);
        *gameObjectBelow = GetGameObjectFromItem(belowItem);
    }
}

QTreeWidgetItem* Hierarchy::SyncHierarchyFromGameObject(GameObject *go)
{
    if (!SceneManager::GetActiveScene()) { return nullptr; }
    if (go->GetHideFlags()->IsOn(HideFlag::HideInHierarchy)) { return nullptr; }

    QTreeWidgetItem *goItem = GetItemFromGameObject(go);
    if (!goItem)
    {
        // New item found! Create item, put name and register into the maps
        goItem = new QTreeWidgetItem();
        goItem->setText(0, go->name.ToQString());

        m_gameObject_To_TreeItem[go] = goItem;
        m_treeItem_To_GameObject[goItem] = go;
    }

    // Refresh go's children. If we find a new child, add it to goItem.
    const List<GameObject*> &children = go->GetChildren();
    for (GameObject* cgo : children)
    {
        QTreeWidgetItem *citem = GetItemFromGameObject(cgo);
        if (citem)
        {
            // Look for possibly new gameObjects inside it
            SyncHierarchyFromGameObject(cgo);
        }
        else // New gameObject found! Add it!
        {
            // Refresh child and keep it into citem
            citem = SyncHierarchyFromGameObject(cgo);
            goItem->addChild(citem); // Add child to hierarchy as go child.
        }
    }

    // Remove items that are not children of go
    List<QTreeWidgetItem*> childrenItem;
    for (int i = 0; i < goItem->childCount(); ++i)
    {
        childrenItem.PushBack(goItem->child(i));
    }

    for (QTreeWidgetItem *childItem : childrenItem)
    {
        GameObject *childItemGo = GetGameObjectFromItem(childItem);
        if (!childItemGo->IsChildOf(go))
        {
            DeleteGameObjectItem(childItemGo);
        }
    }

    // Return it
    return goItem;
}

void Hierarchy::Expand(GameObject *go)
{
    if (go &&
        m_gameObject_To_TreeItem.ContainsKey(go))
    {
        Expand(m_gameObject_To_TreeItem[go]);
    }
}

bool Hierarchy::IsSelected(GameObject *go)
{
    return IsSelected(GetItemFromGameObject(go));
}

bool Hierarchy::IsSelected(QTreeWidgetItem *item)
{
    return item && selectedItems().contains(item);
}

void Hierarchy::ExpandToggle(GameObject *go)
{
    if (go && m_gameObject_To_TreeItem.ContainsKey(go))
    {
        QTreeWidgetItem *item = m_gameObject_To_TreeItem[go];
        if (!item->isExpanded())
        {
            Expand(item);
        }
        else
        {
            item->setExpanded(false);
        }
    }
}

GameObject *Hierarchy::GetFirstSelectedGameObject() const
{
    if (!selectedItems().empty())
    {
        return GetGameObjectFromItem(selectedItems().front());
    }
    return nullptr;
}

void Hierarchy::OnMenuBarCreateEmptyClicked()
{
    m_hContextMenu.OnCreateEmptyClicked();
}


List<GameObject *> Hierarchy::GetSelectedGameObjects(bool excludeInternal)
{
    List<QTreeWidgetItem*> selected = selectedItems().toStdList();
    if (excludeInternal)
    {
        LeaveOnlyOuterMostItems(&selected);
    }

    List<GameObject*> selectedGos;
    for (QTreeWidgetItem *selItem : selected)
    {
        GameObject *go = GetGameObjectFromItem(selItem);
        if (go)
        {
            selectedGos.PushBack(go);
        }
    }
    return selectedGos;
}

void Hierarchy::SelectGameObject(GameObject *go)
{
    SyncHierarchyFromScene();
    UnselectAll();

    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        GameObject *parent = go->parent;
        if (parent)
        {
            Expand(GetItemFromGameObject(parent));
        }

        UnselectAll();
        item->setSelected(true);
        _NotifyHierarchyGameObjectSelectionChanged();
    }
}

void Hierarchy::SelectItemAboveOrBelowSelected(bool above)
{
    SyncHierarchyFromScene();

    QTreeWidgetItem *item = GetFirstSelectedItem();
    if (item)
    {
        QTreeWidgetItem *itemNear = above ? itemAbove(item) : itemBelow(item);
        if (itemNear)
        {
            SelectGameObject( GetGameObjectFromItem(itemNear) );
        }
    }
}

void Hierarchy::OnItemNameChanged(QTreeWidgetItem *item, int column)
{
    //Change the name of the GameObject too
    GameObject *go = GetGameObjectFromItem(item);
    if (go)
    {
        go->SetName(item->text(column));
    }
}

void Hierarchy::OnUpClicked()
{
    ENSURE (state() != State::EditingState);
    SelectItemAboveOrBelowSelected(true);
}

void Hierarchy::OnDownClicked()
{
    ENSURE (state() != State::EditingState);
    SelectItemAboveOrBelowSelected(false);
}

void Hierarchy::OnRightClicked()
{
    ENSURE (state() != State::EditingState);
    QTreeWidgetItem *item = GetFirstSelectedItem();
    if (item) { item->setExpanded(true); }
}

void Hierarchy::OnLeftClicked()
{
    ENSURE (state() != State::EditingState);
    QTreeWidgetItem *item = GetFirstSelectedItem();
    if (item) { item->setExpanded(false); }
}

void Hierarchy::OnSpaceClicked()
{
    ENSURE (state() != State::EditingState);
    GameObject *go = GetFirstSelectedGameObject();
    ExpandToggle(go);
}

void Hierarchy::OnEnterClicked()
{
    if (state() != State::EditingState)
    {
        OnSpaceClicked();
    }
    else
    {
        setCurrentItem(nullptr); // End item editing
    }
}

void Hierarchy::OnRenameClicked()
{
    QTreeWidgetItem *selected = GetFirstSelectedItem();
    if (selected)
    {
        Qt::ItemFlags oldFlags = selected->flags();
        selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);
        editItem(selected, 0);
    }
}

void Hierarchy::OnCopyClicked()
{
    m_hContextMenu.OnCopyClicked();
}

void Hierarchy::OnPasteClicked()
{
    m_hContextMenu.OnPasteClicked();
}

void Hierarchy::OnDuplicateClicked()
{
    m_hContextMenu.OnDuplicateClicked();
}

void Hierarchy::OnDeleteClicked()
{
    m_hContextMenu.OnDeleteClicked();
}

void Hierarchy::DeleteGameObjectItem(GameObject *go)
{
    ENSURE(go);

    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        m_gameObject_To_TreeItem.Remove(go);
        m_treeItem_To_GameObject.Remove(item);
        delete item;
    }
}

void Hierarchy::Print(QTreeWidgetItem *item, const String &indent)
{
    List<QTreeWidgetItem*> itemChildren;
    if (!item)
    {
        for (int i = 0; i < topLevelItemCount(); ++i)
        {
            itemChildren.PushBack(topLevelItem(i));
        }
    }
    else
    {
        for (int i = 0; i < item->childCount(); ++i)
        {
            itemChildren.PushBack(item->child(i));
        }
    }

    for (QTreeWidgetItem *it : itemChildren)
    {
        Debug_Log(indent << it->text(0));
        String indent2 = indent; indent2 += "   ";
        Print(it, indent2);
    }
}

void Hierarchy::dropEvent(QDropEvent *e)
{
    DragDropQTreeWidget::dropEvent(e);
    e->setAccepted(m_hDragDropManager.AcceptDrop());
}

QSize Hierarchy::sizeHint() const
{
    return QSize(150, DragDropQTreeWidget::sizeHint().height());
}

void Hierarchy::OnGameObjectDestroyed(GameObject *destroyed)
{
    DeleteGameObjectItem(destroyed);
}

void Hierarchy::OnSelectionChanged()
{
    Explorer::GetInstance()->clearSelection();
    _NotifyHierarchyGameObjectSelectionChanged();
}

void Hierarchy::_NotifyHierarchyGameObjectSelectionChanged()
{
    List<GameObject*> selectedGameObjects = GetSelectedGameObjects(false);

    WindowEventManager::NotifyHierarchyGameObjectsSelected(selectedGameObjects);

    GameObject *selectedGameObject = GetFirstSelectedGameObject();
    if (selectedGameObject && GetItemFromGameObject(selectedGameObject))
    {
        //Inspector::GetInstance()->ShowInspectable(selectedGameObject);
    }
}

void Hierarchy::_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item,
                                                        int column)
{
    GameObject *selected = GetGameObjectFromItem(item);
    if (selected)
    {
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
