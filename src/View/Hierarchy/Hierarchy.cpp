#include "Hierarchy.h"

#include "Logger.h"
#include "WindowMain.h"
#include "WindowEventManager.h"

Hierarchy::Hierarchy(QWidget *parent) : IDroppableQTreeWidget()
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this ,SLOT(_NotifyHierarchyGameObjectDoubleClicked(
                           QTreeWidgetItem*,int)));

    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

Hierarchy::~Hierarchy()
{
    this->clear();
}

void Hierarchy::ExpandRecursiveUpwards(QTreeWidgetItem *item)
{
    NONULL(item);
    item->setExpanded(true);
    ExpandRecursiveUpwards(item->parent());
}

QTreeWidgetItem* Hierarchy::FillDownwards(GameObject *o)
{
    if (o->IsEditorGameObject()) return nullptr;

    const std::list<GameObject*> children = o->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(o->name));

    for (GameObject* go : children)
    {
        QTreeWidgetItem *c = FillDownwards(go);
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

GameObject *Hierarchy::GetDropTargetGameObject(QDropEvent *e) const
{
    QTreeWidgetItem *targetItem = itemAt(e->pos());
    GameObject *targetGameObject = nullptr;
    if (targetItem)
    {
        targetGameObject = GetGameObjectFromItem(targetItem);
        DropIndicatorPosition dropPos = dropIndicatorPosition();
        if (dropPos == BelowItem || dropPos == AboveItem)
        {
            //Not putting inside, but below or above. Thus take its parent.
            targetItem = targetItem->parent();
            if (targetItem) targetGameObject = GetGameObjectFromItem(targetItem);
            else targetGameObject = Scene::GetCurrentScene();
        }
    }
    else
    {
        targetGameObject = Scene::GetCurrentScene();
    }

    return targetGameObject;
}

void Hierarchy::UnselectAll()
{
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        item->setSelected(false);
    }

    _NotifyHierarchyGameObjectSelectionChanged();
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

    m_gameObjectToTreeItem.clear();
    m_treeItemToGameObject.clear();
    clear();

    std::list<GameObject*> sceneChildren = Scene::GetCurrentScene()->GetChildren();
    for (GameObject* go : sceneChildren)
    {
        if (!go->IsEditorGameObject())
        {
            this->addTopLevelItem( FillDownwards(go) );
        }
    }
}

GameObject *Hierarchy::GetFirstSelectedGameObject() const
{
    if (!selectedItems().empty()) return GetGameObjectFromItem(selectedItems().front());
    return nullptr;
}


void Hierarchy::OnChildAdded(GameObject *child)
{
    if (child->IsEditorGameObject()) return;

    QTreeWidgetItem *item = FillDownwards(child); NONULL(item);

    if (child->parent)
    {
        if (!child->parent->IsScene())
        {
            QTreeWidgetItem *parentItem = GetItemFromGameObject(child->parent);
            if (parentItem)
            {
                parentItem->addChild(item);
                ExpandRecursiveUpwards(GetItemFromGameObject(child->parent));
            }
        }
        else
        {
            addTopLevelItem(item);
        }
    }
}

void Hierarchy::OnChildChangedParent(GameObject *child, GameObject *previousParent)
{
}

void Hierarchy::OnChildRemoved(GameObject *child)
{
    if (child->IsEditorGameObject()) return;

    QTreeWidgetItem *item = GetItemFromGameObject(child);
    NONULL(item);

    if (item->isSelected())
    {
        item->setSelected(false);
    }

    delete item;

    m_treeItemToGameObject.erase(item);
    m_gameObjectToTreeItem.erase(child);
}

void Hierarchy::dropEvent(QDropEvent *e)
{
    IDroppableQTreeWidget::dropEvent(e);

    QTreeWidgetItem *targetItem = itemAt(e->pos()); NONULL(targetItem);
    GameObject *targetGameObject = GetDropTargetGameObject(e);

    if (e->source() == this) // From Hierarchy To Hierarchy
    {
        std::list<QTreeWidgetItem*> sourceItems = selectedItems().toStdList();
        LeaveOnlyOuterMostItems(&sourceItems);
        if (!sourceItems.empty())
        {
            for (QTreeWidgetItem *sourceItem : sourceItems)
            {
                if (sourceItem != targetItem)
                {
                    GameObject *sourceGameObject = GetGameObjectFromItem(sourceItem);
                    if (sourceGameObject && targetGameObject  && sourceGameObject->parent)
                    {
                        sourceGameObject->SetParent(targetGameObject, true);
                    }
                }
            }
        }
    }
    e->accept();
}

void Hierarchy::OnDropFromExplorer(const File &f, QDropEvent *e)
{
    if (f.GetRelativePath().length() == 0) return;

    GameObject *targetGameObject = GetDropTargetGameObject(e); NONULL(targetGameObject);
    if (f.IsPrefabAsset())
    {
        Prefab *prefab = new Prefab();
        prefab->ReadXMLInfoFromString(f.GetContents());
        GameObject *go = prefab->Instantiate();
        go->SetParent(targetGameObject);
    }
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
        if (this->selectedItems().length() <= 0) return;
        QTreeWidgetItem *selected = this->selectedItems().at(0);
        if (selected )
        {
            Qt::ItemFlags oldFlags = selected->flags();
            selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);

            this->editItem(selected, 0); // Name can be edited now

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
    this->clearSelection();

    QTreeWidgetItem *item = GetItemFromGameObject(go);
    if (item)
    {
        GameObject *parent = go->parent;
        if (parent)
        {
            ExpandRecursiveUpwards(GetItemFromGameObject(parent));
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
}

void Hierarchy::_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column)
{
    GameObject *selected = GetGameObjectFromItem(item);
    if (selected)
    {
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
