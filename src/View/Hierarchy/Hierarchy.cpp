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
            this ,SLOT(_NotifyHierarchyGameObjectDoubleClicked(
                           QTreeWidgetItem*,int)));
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

QTreeWidgetItem* Hierarchy::PopulateItemGameObject(GameObject *go)
{
    if (go->IsEditorGameObject()) return nullptr;

    const std::list<GameObject*> children = go->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(go->name));

    for (GameObject* cgo : children)
    {
        QTreeWidgetItem *c = PopulateItemGameObject(cgo);
        if (c)
        {
            eRoot->addChild(c);
        }
    }

    m_gameObjectToTreeItem[go] = eRoot;
    m_treeItemToGameObject[eRoot] = go;

    return eRoot;
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
    if (m_treeItemToGameObject.find(item) != m_treeItemToGameObject.end())
    {
        return m_treeItemToGameObject[item];
    }
    return nullptr;
}

QTreeWidgetItem *Hierarchy::GetItemFromGameObject(GameObject *go) const
{
    if (m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        return m_gameObjectToTreeItem[go];
    }
    return nullptr;
}

void Hierarchy::UnselectAll()
{
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        if (item->isSelected())
        {
            item->setSelected(false);
        }
    }

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

void Hierarchy::Clear()
{
    m_gameObjectToTreeItem.clear();
    m_treeItemToGameObject.clear();
    clear();
}

void Hierarchy::Refresh()
{
    NONULL(SceneManager::GetActiveScene());

    // Save expanded and selected gameObject items
    GameObject *selectedGameObject = nullptr;
    std::list<GameObject*> expandedGameObjects;
    for (auto it : m_gameObjectToTreeItem)
    {
        QTreeWidgetItem *item = it.second;
        if (item->isExpanded())
        {
            expandedGameObjects.push_back(it.first);
        }

        if (item->isSelected())
        {
            selectedGameObject = it.first;
        }
    }

    Clear();

    // Add top level items and populate them recursively
    std::list<GameObject*> sceneChildren = SceneManager::GetActiveScene()->GetChildren();
    for (GameObject* go : sceneChildren)
    {
        if (!go->IsEditorGameObject())
        {
            addTopLevelItem( PopulateItemGameObject(go) );
        }
    }

    // Restore expanded gameObject items
    for (GameObject *expandedGameObject : expandedGameObjects)
    {
        if (m_gameObjectToTreeItem.find(expandedGameObject) !=
            m_gameObjectToTreeItem.end())
        {
            QTreeWidgetItem *item = m_gameObjectToTreeItem[expandedGameObject];
            item->setExpanded(true);
        }
    }

    // Restore selected gameObject
    if (selectedGameObject &&
        m_gameObjectToTreeItem.find(selectedGameObject) !=
            m_gameObjectToTreeItem.end())
    {
        m_gameObjectToTreeItem[selectedGameObject]->setSelected(true);
    }
}

void Hierarchy::Expand(GameObject *go)
{
    if (go &&
        m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        Expand(m_gameObjectToTreeItem[go]);
    }
}

void Hierarchy::ExpandTrigger(GameObject *go)
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
            ExpandTrigger(go);
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
        selectedGos.push_back(GetGameObjectFromItem(selItem));
    }

    return selectedGos;
}

void Hierarchy::SelectGameObject(GameObject *go)
{
    clearSelection();

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
    QTreeWidgetItem *item = GetFirstSelectedItem();
    if (item)
    {
        QTreeWidgetItem *itemUp = above ? itemAbove(item) : itemBelow(item);
        if (itemUp)
        {
            UnselectAll();
            itemUp->setSelected(true);
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

void Hierarchy::OnGameObjectDestroyed(GameObject *destroyed)
{
    NONULL(destroyed);
    if (m_gameObjectToTreeItem.find(destroyed) != m_gameObjectToTreeItem.end())
    {
        QTreeWidgetItem *item = m_gameObjectToTreeItem[destroyed];
        m_gameObjectToTreeItem.erase(m_gameObjectToTreeItem.find(destroyed));
        m_treeItemToGameObject.erase(m_treeItemToGameObject.find(item));
        delete item;
    }
}

void Hierarchy::OnSelectionChanged()
{
    _NotifyHierarchyGameObjectSelectionChanged();
}

void Hierarchy::_NotifyHierarchyGameObjectSelectionChanged()
{
    std::list<GameObject*> selectedGameObjects;
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        GameObject *go = GetGameObjectFromItem(item);
        if (go)
        {
            selectedGameObjects.push_back(go);
        }
    }

    /* Expand with one click
    if (selectedGameObjects.size() == 1)
    {
        GameObject *go = selectedGameObjects.front();
        QTreeWidgetItem *item = gameObjectToTreeItem[go];
        item->setExpanded(true);
    }*/

    WindowEventManager::NotifyHierarchyGameObjectsSelected(selectedGameObjects);
    Inspector::GetInstance()->ShowGameObjectInfo(GetFirstSelectedGameObject());
}

void Hierarchy::_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column)
{
    GameObject *selected = GetGameObjectFromItem(item);
    if (selected)
    {
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
