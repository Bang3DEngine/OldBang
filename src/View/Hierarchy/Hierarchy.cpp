#include "Hierarchy.h"

#include "Debug.h"
#include "EditorWindow.h"
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
            this, SLOT(RefreshFromScene()));
    m_refreshTimer.start();
}

Hierarchy::~Hierarchy()
{
    clear();
}

void Hierarchy::Expand(QTreeWidgetItem *item)
{
    NONULL(item);
    item->setExpanded(true);
    Expand(item->parent());
}

void Hierarchy::LeaveOnlyOuterMostItems(std::list<QTreeWidgetItem*> *items)
{
    //For each item, it will be a top level item,
    //if none of the selected items is its parent
    std::list<QTreeWidgetItem*> result;
    for (QTreeWidgetItem *item : *items)
    {
        bool hasItsParentInTheList = false;
        QTreeWidgetItem *parent = item->parent();
        if (parent)
        {
            for (QTreeWidgetItem *item2 : *items)
            {
                if (parent == item2)
                {
                    hasItsParentInTheList = true;
                    break;
                }
            }
        }

        if (!hasItsParentInTheList)
        {
            result.push_back(item);
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
    if (m_treeItemToGameObject.find(item) != m_treeItemToGameObject.end())
    {
        return m_treeItemToGameObject[item];
    }
    return nullptr;
}

QTreeWidgetItem *Hierarchy::GetItemFromGameObject(GameObject *go) const
{
    if (!go) return nullptr;
    if (m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        QTreeWidgetItem *item = m_gameObjectToTreeItem[go];
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
    m_gameObjectToTreeItem.clear();
    m_treeItemToGameObject.clear();
    clear();
}

void Hierarchy::RefreshFromScene()
{
    Scene *scene = SceneManager::GetActiveScene(); NONULL(scene);

    // refresh go's children. If we find a new child, add it to topLevel.
    const std::list<GameObject*> sceneChildren = scene->GetChildren();
    for (GameObject* child : sceneChildren)
    {
        QTreeWidgetItem *childItem = GetItemFromGameObject(child);
        if (!childItem)
        {   // New child item !!!
            childItem = Refresh(child);
            addTopLevelItem(childItem);
            m_gameObjectToTreeItem[child] = childItem;
            m_treeItemToGameObject[childItem] = child;
        }
        else
        {
            Refresh(child); // Just refresh
        }
    }
}

QTreeWidgetItem* Hierarchy::Refresh(GameObject *go)
{
    if (!SceneManager::GetActiveScene())
    {
        return nullptr;
    }

    if (go->IsEditorGameObject()) return nullptr;


    QTreeWidgetItem *goItem = GetItemFromGameObject(go);
    if (!goItem)
    {
        // New item found! Create item, put name and register into the maps
        goItem = new QTreeWidgetItem();
        goItem->setText(0, QString::fromStdString(go->name));

        m_gameObjectToTreeItem[go] = goItem;
        m_treeItemToGameObject[goItem] = go;
    }

    // refresh go's children. If we find a new child, add it to goItem.
    const std::list<GameObject*> children = go->GetChildren();
    for (GameObject* cgo : children)
    {
        QTreeWidgetItem *citem = GetItemFromGameObject(cgo);
        if (citem)
        {
            Refresh(cgo); // Look for possibly new gameObjects inside it
        }
        else // New gameObject found! Add it!
        {
            citem = Refresh(cgo);    // Refresh child and keep it into citem
            goItem->addChild(citem); // Add child to hierarchy as go child.
        }
    }

    // Return it
    return goItem;
}

void Hierarchy::Expand(GameObject *go)
{
    if (go &&
        m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        Expand(m_gameObjectToTreeItem[go]);
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
        m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        QTreeWidgetItem *item = m_gameObjectToTreeItem[go];
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


std::list<GameObject *> Hierarchy::GetSelectedGameObjects(bool excludeInternal)
{
    std::list<QTreeWidgetItem*> selected = selectedItems().toStdList();
    if (excludeInternal)
    {
        LeaveOnlyOuterMostItems(&selected);
    }

    std::list<GameObject*> selectedGos;
    for (QTreeWidgetItem *selItem : selected)
    {
        GameObject *go = GetGameObjectFromItem(selItem);
        if (go)
        {
            selectedGos.push_back(go);
        }
    }
    return selectedGos;
}

void Hierarchy::SelectGameObject(GameObject *go)
{
    RefreshFromScene();
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
    RefreshFromScene();

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
    NONULL(go);

    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        m_gameObjectToTreeItem.erase(m_gameObjectToTreeItem.find(go));
        m_treeItemToGameObject.erase(m_treeItemToGameObject.find(item));

        // Try to delete children's items too
        for (int i = 0; i < item->childCount(); ++i)
        {
            QTreeWidgetItem *childItem = item->child(i);
            GameObject *childGo = GetGameObjectFromItem(childItem);
            if (childGo)
            {
                DeleteGameObjectItem(childGo);
            }
        }
        delete item;
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
    std::list<GameObject*> selectedGameObjects = GetSelectedGameObjects(false);

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
