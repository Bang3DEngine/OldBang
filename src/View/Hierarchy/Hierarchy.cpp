#include "Hierarchy.h"

#include "Logger.h"
#include "WindowMain.h"
#include "WindowEventManager.h"

Hierarchy::Hierarchy(QWidget *parent) : m_hDragDropManager(this)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));

    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(OnSelectionChanged()));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this ,SLOT(_NotifyHierarchyGameObjectDoubleClicked(
                           QTreeWidgetItem*,int)));

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnCustomContextMenuRequested(QPoint)));
}

Hierarchy::~Hierarchy()
{
    this->clear();
}

void Hierarchy::Expand(QTreeWidgetItem *item)
{
    NONULL(item);
    item->setExpanded(true);
    Expand(item->parent());
}

QTreeWidgetItem* Hierarchy::PopulateItemGameObject(GameObject *o)
{
    if (o->IsEditorGameObject()) return nullptr;

    const std::list<GameObject*> children = o->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(o->name));

    for (GameObject* go : children)
    {
        QTreeWidgetItem *c = PopulateItemGameObject(go);
        if (c)
        {
            eRoot->addChild(c);
        }
    }

    m_gameObjectToTreeItem[o] = eRoot;
    m_treeItemToGameObject[eRoot] = o;

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
        item->setSelected(false);
    }

    _NotifyHierarchyGameObjectSelectionChanged();
}

void Hierarchy::dropEvent(QDropEvent *e)
{
    DragDropQTreeWidget::dropEvent(e);
    e->ignore();
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
    return WindowMain::GetInstance()->widgetHierarchy;
}

void Hierarchy::Refresh()
{
    NONULL(Scene::GetCurrentScene());

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

    m_gameObjectToTreeItem.clear();
    m_treeItemToGameObject.clear();
    clear();

    // Add top level items and populate them recursively
    std::list<GameObject*> sceneChildren = Scene::GetCurrentScene()->GetChildren();
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
    if (selectedGameObject)
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

GameObject *Hierarchy::GetFirstSelectedGameObject() const
{
    if (!selectedItems().empty()) return GetGameObjectFromItem(selectedItems().front());
    return nullptr;
}

void Hierarchy::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if (clickedAction == MenuBar::Action::CreateEmptyGameObject)
    {
        OnContextMenuCreateEmptyClicked();
    }
}

void Hierarchy::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Delete) // Delete item
    {
        OnContextMenuDeleteClicked();
    }
    else if (e->key() == Qt::Key_F2) // Edit name
    {
        if (selectedItems().length() <= 0) return;
        QTreeWidgetItem *selected = this->selectedItems().at(0);
        if (selected)
        {
            Qt::ItemFlags oldFlags = selected->flags();
            selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);

            editItem(selected, 0); // Name can be edited now

            //Restore not editable by click
            selected->setFlags(oldFlags);
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

void Hierarchy::OnItemNameChanged(QTreeWidgetItem *item, int column)
{
    //Change the name of the GameObject too
    GameObject *go = GetGameObjectFromItem(item);
    if (go)
    {
        go->SetName(item->text(column).toStdString());
    }
}

void Hierarchy::OnContextMenuCreateEmptyClicked()
{
    GameObject *empty = new GameObject();
    empty->SetName("Empty");

    foreach(QTreeWidgetItem *item, selectedItems())
    {
        GameObject *selected = GetGameObjectFromItem(item);
        empty->SetParent(selected);
    }

    if (selectedItems().size() == 0)
    {
        empty->SetParent(Scene::GetCurrentScene());
    }
}

void Hierarchy::OnContextMenuCopyClicked()
{
    std::list<GameObject*> whatToCopy = GetSelectedGameObjects(true);
    ClipboardGameObject::CopyGameObjects(whatToCopy);
}

void Hierarchy::OnContextMenuPasteClicked()
{
    std::list<GameObject*> selected = GetSelectedGameObjects(false);
    if (selected.size() > 0)
    {
        for (GameObject *sel : selected)
        {
            ClipboardGameObject::PasteCopiedGameObjectsInto(sel);
        }
    }
    else
    {
        ClipboardGameObject::PasteCopiedGameObjectsInto(Scene::GetCurrentScene());
    }
}

void Hierarchy::OnContextMenuDuplicateClicked()
{
    std::list<GameObject*> selected = GetSelectedGameObjects(true);
    ClipboardGameObject::CopyGameObjects(selected);
    ClipboardGameObject::DuplicateCopiedGameObjects();
}

void Hierarchy::OnContextMenuDeleteClicked()
{
    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    LeaveOnlyOuterMostItems(&items);
    foreach(QTreeWidgetItem *item, items)
    {
        GameObject *selected = GetGameObjectFromItem(item);
        delete selected;
    }
}

void Hierarchy::OnContextMenuCreatePrefab()
{
    GameObject *go = GetFirstSelectedGameObject();

    FileDialog fd("Create Prefab...", Prefab::GetFileExtensionStatic());
    std::string filename = fd.GetSaveFilename(go->name);
    if (filename != "")
    {
        Prefab *prefab = new Prefab(go);
        FileWriter::WriteToFile(filename, prefab);
        delete prefab;
    }
}

void Hierarchy::OnCustomContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = itemAt(point);
    QMenu contextMenu(tr("GameObject hierarchy context menu"), this);

    QAction actionCreateEmpty("Create empty", this);
    QAction actionCopy("Copy", this);
    QAction actionPaste("Paste", this);
    actionPaste.setEnabled(ClipboardGameObject::HasSomethingCopied());
    QAction actionDuplicate("Duplicate", this);
    QAction actionCreatePrefab("Create Prefab...", this);
    QAction actionDelete("Delete", this);

    connect(&actionCreateEmpty, SIGNAL(triggered()), this, SLOT(OnContextMenuCreateEmptyClicked()));
    connect(&actionCopy, SIGNAL(triggered()), this, SLOT(OnContextMenuCopyClicked()));
    connect(&actionPaste, SIGNAL(triggered()), this, SLOT(OnContextMenuPasteClicked()));
    connect(&actionDuplicate, SIGNAL(triggered()), this, SLOT(OnContextMenuDuplicateClicked()));
    connect(&actionCreatePrefab, SIGNAL(triggered()), this, SLOT(OnContextMenuCreatePrefab()));
    connect(&actionDelete, SIGNAL(triggered()), this, SLOT(OnContextMenuDeleteClicked()));

    if (item)
    {
        contextMenu.addAction(&actionCreateEmpty);
        contextMenu.addAction(&actionCopy);
        contextMenu.addAction(&actionPaste);
        contextMenu.addAction(&actionDuplicate);
        contextMenu.addAction(&actionCreatePrefab);
        contextMenu.addAction(&actionDelete);
    }
    else
    {
        contextMenu.addAction(&actionCreateEmpty);
        contextMenu.addAction(&actionPaste);
    }

    contextMenu.exec(mapToGlobal(point));
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
