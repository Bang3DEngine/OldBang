#include "Hierarchy.h"

#include <chrono>
#include <algorithm>

#include "Debug.h"
#include "Debug.h"
#include "Scene.h"
#include "GameObject.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "ShortcutManager.h"
#include "WindowEventManager.h"

Hierarchy::Hierarchy(QWidget *parent) :
    m_hContextMenu(this), m_hDragDropManager(this)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));

    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(OnSelectionChanged()));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(_NotifyHierarchyGameObjectDoubleClicked(
                           QTreeWidgetItem*,int)));

    m_refreshTimer.setInterval(1000);
    connect(&m_refreshTimer, SIGNAL(timeout()),
            this, SLOT(UpdateHierarchyFromScene()));
    m_refreshTimer.start();
}

Hierarchy::~Hierarchy()
{
    clear();
}

void Hierarchy::Expand(QTreeWidgetItem *item)
{
    EXISTS(item);
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
    if (!item) return nullptr;

    if (m_treeItem_To_GameObject.ContainsKey(item))
    {
        return m_treeItem_To_GameObject[item];
    }
    return nullptr;
}

QTreeWidgetItem *Hierarchy::GetItemFromGameObject(GameObject *go) const
{
    if (!go) return nullptr;
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

void Hierarchy::dropEvent(QDropEvent *e)
{
    DragDropQTreeWidget::dropEvent(e);
    e->ignore();
}

void Hierarchy::keyPressEvent(QKeyEvent *e)
{
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
        item->setText(0, QString::fromStdString(go->name) );
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

void Hierarchy::UpdateHierarchyFromScene()
{
    Scene *scene = SceneManager::GetActiveScene(); EXISTS(scene);

    // Refresh go's children. If we find a new child, add it to topLevel.
    const List<GameObject*> sceneChildren = scene->GetChildren();
    for (GameObject* child : sceneChildren)
    {
        QTreeWidgetItem *childItem = GetItemFromGameObject(child);
        if (!childItem)
        {
            // New child item !!!
            childItem = Update(child);
            addTopLevelItem(childItem);
            m_gameObject_To_TreeItem[child] = childItem;
            m_treeItem_To_GameObject[childItem] = child;
        }
        else
        {
            Update(child); // Just refresh
        }
    }
}

void Hierarchy::UpdateSceneFromHierarchy()
{
    // First case, direct children of scene (top level items)
    Scene *scene = SceneManager::GetActiveScene();
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *childItem = topLevelItem(i);
        UpdateGameObjectFromHierarchy(childItem); // Recursive call

        GameObject *childItemGo = GetGameObjectFromItem(childItem);
        childItemGo->SetParent(scene, true, nullptr);
    }
}

void Hierarchy::UpdateGameObjectFromHierarchy(QTreeWidgetItem *goItem)
{
    // Iterate all items, and update
    GameObject *go = GetGameObjectFromItem(goItem);
    for (int i = 0; i < goItem->childCount(); ++i)
    {
        QTreeWidgetItem *childItem = goItem->child(i);
        UpdateGameObjectFromHierarchy(childItem); // Recursive call

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

QTreeWidgetItem* Hierarchy::Update(GameObject *go)
{
    if (!SceneManager::GetActiveScene()) { return nullptr; }
    if (go->IsEditorGameObject()) { return nullptr; }

    QTreeWidgetItem *goItem = GetItemFromGameObject(go);
    if (!goItem)
    {
        // New item found! Create item, put name and register into the maps
        goItem = new QTreeWidgetItem();
        goItem->setText(0, QString::fromStdString(go->name));

        m_gameObject_To_TreeItem[go] = goItem;
        m_treeItem_To_GameObject[goItem] = go;
    }

    // Refresh go's children. If we find a new child, add it to goItem.
    const List<GameObject*> children = go->GetChildren();
    for (GameObject* cgo : children)
    {
        QTreeWidgetItem *citem = GetItemFromGameObject(cgo);
        if (citem)
        {
            Update(cgo); // Look for possibly new gameObjects inside it
        }
        else // New gameObject found! Add it!
        {
            citem = Update(cgo);    // Refresh child and keep it into citem
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
    if (go &&
        m_gameObject_To_TreeItem.ContainsKey(go))
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
    if (!selectedItems().empty()) return GetGameObjectFromItem(selectedItems().front());
    return nullptr;
}

void Hierarchy::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if (clickedAction == MenuBar::Action::CreateEmptyGameObject)
    {
        m_hContextMenu.OnCreateEmptyClicked();
    }
}

void Hierarchy::OnShortcutPressed()
{
    if (hasFocus())
    {
        if ( ShortcutManager::IsPressed(Input::Key::Enter) ||
             ShortcutManager::IsPressed(Input::Key::Space))
        {
            GameObject *go = GetFirstSelectedGameObject();
            ExpandToggle(go);
        }
        else if ( ShortcutManager::IsPressed(Input::Key::Up))
        {
            SelectItemAboveOrBelowSelected(true);
        }
        else if ( ShortcutManager::IsPressed(Input::Key::Down))
        {
            SelectItemAboveOrBelowSelected(false);
        }
        else if ( ShortcutManager::IsPressed(Input::Key::Left))
        {
            QTreeWidgetItem *item = GetFirstSelectedItem();
            if (item)
            {
                item->setExpanded(false);
            }
        }
        else if ( ShortcutManager::IsPressed(Input::Key::Right))
        {
            QTreeWidgetItem *item = GetFirstSelectedItem();
            if (item)
            {
                item->setExpanded(true);
            }
        }
        else if ( ShortcutManager::IsPressed(Input::Key::F2) )
        { // Edit name
            if (selectedItems().length() <= 0) return;
            QTreeWidgetItem *selected = selectedItems().at(0);
            if (selected)
            {
                Qt::ItemFlags oldFlags = selected->flags();
                selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);

                editItem(selected, 0); // Name can be edited now
            }
        }
        else if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::C}) )
        { // Copy
            m_hContextMenu.OnCopyClicked();
        }
        else if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::V}) )
        { // Paste
            m_hContextMenu.OnPasteClicked();
        }
        else if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::D}) )
        { // Duplicate
            m_hContextMenu.OnDuplicateClicked();
        }
        else if (ShortcutManager::IsPressed(Input::Key::Delete)) // Delete item
        { // Delete
            m_hContextMenu.OnDeleteClicked();
        }
    }
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
    UpdateHierarchyFromScene();
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
    UpdateHierarchyFromScene();

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
        go->SetName(item->text(column).toStdString());
    }
}

void Hierarchy::DeleteGameObjectItem(GameObject *go)
{
    EXISTS(go);

    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        m_gameObject_To_TreeItem.Remove(go);
        m_treeItem_To_GameObject.Remove(item);

        // Try to delete children's items too
        /*
        for (int i = 0; i < item->childCount(); ++i)
        {
            QTreeWidgetItem *childItem = item->child(i);
            GameObject *childGo = GetGameObjectFromItem(childItem);
            if (childGo)
            {
                DeleteGameObjectItem(childGo);
            }
        }
        */
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

void Hierarchy::OnGameObjectDestroyed(GameObject *destroyed)
{
    DeleteGameObjectItem(destroyed);
}

void Hierarchy::OnSelectionChanged()
{
    _NotifyHierarchyGameObjectSelectionChanged();
}

void Hierarchy::_NotifyHierarchyGameObjectSelectionChanged()
{
    List<GameObject*> selectedGameObjects = GetSelectedGameObjects(false);

    WindowEventManager::NotifyHierarchyGameObjectsSelected(selectedGameObjects);

    GameObject *selectedGameObject = GetFirstSelectedGameObject();
    if (selectedGameObject && GetItemFromGameObject(selectedGameObject))
    {
        Inspector::GetInstance()->ShowGameObjectInfo(selectedGameObject);
    }
}

void Hierarchy::_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column)
{
    GameObject *selected = GetGameObjectFromItem(item);
    if (selected)
    {
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
