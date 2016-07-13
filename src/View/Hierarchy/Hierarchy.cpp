#include "Hierarchy.h"

#include "Logger.h"
#include "WindowMain.h"
#include "WindowEventManager.h"
#include "EditorScene.h"

Hierarchy::Hierarchy(QWidget *parent)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(OnSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this ,SLOT(_NotifyHierarchyGameObjectDoubleClicked(
                           QTreeWidgetItem*,int)));

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
    if(o->IsEditorGameObject()) return nullptr;

    const std::list<GameObject*> children = o->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(o->GetName()));

    for(GameObject* go : children)
    {
        QTreeWidgetItem *c = FillDownwards(go);
        if(c)
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
    for(QTreeWidgetItem *item : *items)
    {
        bool hasItsParentInTheList = false;
        QTreeWidgetItem *parent = item->parent();
        if(parent)
        {
            for(QTreeWidgetItem *item2 : *items)
            {
                if(parent == item2)
                {
                    hasItsParentInTheList = true;
                    break;
                }
            }
        }

        if(!hasItsParentInTheList)
        {
            result.push_back(item);
        }
    }
    *items = result;
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
    if(m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        QTreeWidgetItem *item = m_gameObjectToTreeItem[go];
        item->setText(0, QString::fromStdString(go->GetName()) );
    }
}

Hierarchy *Hierarchy::GetInstance()
{
    return WindowMain::GetInstance()->widgetHierarchy;
}

void Hierarchy::Refresh()
{
    p_currentScene = Canvas::GetInstance()->GetCurrentScene(); NONULL(p_currentScene);

    m_gameObjectToTreeItem.clear();
    m_treeItemToGameObject.clear();
    this->clear();

    std::list<GameObject*> sceneChildren = p_currentScene->GetChildren();
    for(GameObject* go : sceneChildren)
    {
        if(!go->IsEditorGameObject())
        {
            this->addTopLevelItem( FillDownwards(go) );
        }
    }
}

GameObject *Hierarchy::GetFirstSelectedGameObject() const
{
    if(!selectedItems().empty()) return m_treeItemToGameObject[selectedItems().at(0)];
    return nullptr;
}


void Hierarchy::OnChildAdded(GameObject *child)
{
    if(child->IsEditorGameObject()) return;

    GameObject *parent = child->GetParent();
    QTreeWidgetItem *item = FillDownwards(child); NONULL(item);

    if(parent && !parent->IsScene() &&
       m_gameObjectToTreeItem.find(parent) != m_gameObjectToTreeItem.end())
    {
        m_gameObjectToTreeItem[parent]->addChild(item);
        ExpandRecursiveUpwards(m_gameObjectToTreeItem[parent]);
    }
    else
    {
        addTopLevelItem(item);
    }
}

void Hierarchy::OnChildChangedParent(GameObject *child, GameObject *previousParent)
{
}

void Hierarchy::OnChildRemoved(GameObject *child)
{
    if(child->IsEditorGameObject()) return;

    Logger_Log("OnChildRemoved: " << child);
    QTreeWidgetItem *item = m_gameObjectToTreeItem[child]; NONULL(item);
    if (!item->parent())
    {
        removeItemWidget(item, 0);
    }
    else
    {
        item->parent()->removeChild(item);
    }

    m_treeItemToGameObject.erase(item);
    m_gameObjectToTreeItem.erase(child);
}

void Hierarchy::dropEvent(QDropEvent *event)
{
    std::list<QTreeWidgetItem*> sourceItems = selectedItems().toStdList();
    LeaveOnlyOuterMostItems(&sourceItems);

    QTreeWidgetItem *targetItem = itemAt(event->pos());
    if(targetItem  && !sourceItems.empty())
    {
        GameObject *target = m_treeItemToGameObject[targetItem];
        DropIndicatorPosition dropPos = dropIndicatorPosition();
        if (dropPos == BelowItem || dropPos == AboveItem)
        {
            //Not putting inside, but below or above. Thus take its parent.
            targetItem = targetItem->parent();
            if(targetItem) target = m_treeItemToGameObject[targetItem];
            else target = p_currentScene;
        }

        for(QTreeWidgetItem *sourceItem : sourceItems)
        {
            if(sourceItem != targetItem)
            {
                GameObject *source = m_treeItemToGameObject[sourceItem];
                if(source && target && source->GetParent())
                {
                    source->GetParent()->MoveChild(source, target);
                }
            }
        }
    }

    QTreeWidget::dropEvent(event);
    event->acceptProposedAction();
}

void Hierarchy::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if(clickedAction == MenuBar::Action::CreateEmptyGameObject)
    {
        OnContextMenuCreateEmptyClicked();
    }
}

void Hierarchy::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Delete) // Delete item
    {
        OnContextMenuDeleteClicked();
    }
    else if(e->key() == Qt::Key_F2) // Edit name
    {
        if(this->selectedItems().length() <= 0) return;
        QTreeWidgetItem *selected = this->selectedItems().at(0);
        if(selected )
        {
            Qt::ItemFlags oldFlags = selected->flags();
            selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);

            this->editItem(selected, 0); // Name can be edited now

            //Restore not editable by click
            selected->setFlags(oldFlags);
        }
    }
}

void Hierarchy::SelectGameObject(GameObject *go)
{
    this->clearSelection();


    if(m_gameObjectToTreeItem.find(go) != m_gameObjectToTreeItem.end())
    {
        GameObject *parent = go->GetParent();
        if(parent )
        {
            ExpandRecursiveUpwards(m_gameObjectToTreeItem[parent]);
        }

        UnselectAll();
        m_gameObjectToTreeItem[go]->setSelected(true);
        _NotifyHierarchyGameObjectSelectionChanged();
    }
}

void Hierarchy::OnItemNameChanged(QTreeWidgetItem *item, int column)
{
    if(m_treeItemToGameObject.find(item) != m_treeItemToGameObject.end())
    {   //Change the name of the GameObject itself, not just the item text
        m_treeItemToGameObject[item]->SetName(item->text(column).toStdString());
    }
}

void Hierarchy::OnContextMenuCreateEmptyClicked()
{
    GameObject *empty = new GameObject();
    empty->SetName("Empty");

    foreach(QTreeWidgetItem *item, selectedItems())
    {
        GameObject *selected = m_treeItemToGameObject[item];
        selected->AddChild(empty);
    }

    if(selectedItems().size() == 0)
    {
        p_currentScene->AddChild(empty);
    }
}

void Hierarchy::OnContextMenuCopyClicked()
{
    m_copiedGameObjects.clear();

    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    LeaveOnlyOuterMostItems(&items);
    for (QTreeWidgetItem *item : items)
    {
        GameObject *selected = m_treeItemToGameObject[item];
        GameObject *copy = static_cast<GameObject*>(selected->Clone());
        m_copiedGameObjects.push_back(copy);
    }
}

void Hierarchy::OnContextMenuPasteClicked()
{
    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    for (QTreeWidgetItem *item : items)
    {
        // We first store all the copies of the copies into a local list,
        // to avoid modifying them while pasting if we dont do it in 2 steps like now.
        std::list<ICloneable*> localCopies;
        for (ICloneable *copy : m_copiedGameObjects)
        {
            localCopies.push_back(copy->Clone());
        }

        GameObject *selected = m_treeItemToGameObject[item];
        for (ICloneable *copy : localCopies)
        {
            GameObject *paste = static_cast<GameObject*>(copy->Clone());
            selected->AddChild(paste);
        }
    }
}

void Hierarchy::OnContextMenuDuplicateClicked()
{
    OnContextMenuCopyClicked();

    // In 2 steps to avoid modifying copies while pasting
    std::list<ICloneable*> localCopies;
    for (ICloneable *copy : m_copiedGameObjects)
    {
        localCopies.push_back(copy->Clone());
    }

    for (ICloneable *copy : localCopies)
    {
        GameObject *copygo = static_cast<GameObject*>(copy);
        GameObject *paste = static_cast<GameObject*>(copy->Clone());
        GameObject *parent = copygo->GetParent() ? copygo->GetParent() : p_currentScene;
        parent->AddChild(paste);
    }
}

void Hierarchy::OnContextMenuDeleteClicked()
{
    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    LeaveOnlyOuterMostItems(&items);
    foreach(QTreeWidgetItem *item, items)
    {
        GameObject *selected = m_treeItemToGameObject[item];
        if(selected->parent)
        {
            selected->parent->RemoveChild(selected);
        }
    }
}

void Hierarchy::OnContextMenuCreatePrefab()
{
    GameObject *e = GetFirstSelectedGameObject();
    Prefab *prefab = new Prefab(e);

    std::string ext = Prefab::GetFileExtensionStatic();
    FileDialog fd("Create Prefab...", Prefab::GetFileExtensionStatic());
    std::string filename = fd.GetSaveFilename(e->GetName());
    if(filename != "")
    {
        std::fstream f;
        f.open(filename, std::fstream::out);
        if(f.is_open())
        {
            prefab->Write(f);
            f.close();
        }
    }

    delete prefab;
}

void Hierarchy::OnCustomContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = itemAt(point);
    QMenu contextMenu(tr("GameObject hierarchy context menu"), this);

    GameObject *e = m_treeItemToGameObject[item];

    QAction actionCreateEmpty("Create empty", this);
    QAction actionCopy("Copy", this);
    QAction actionPaste("Paste", this);
    actionPaste.setEnabled(m_copiedGameObjects.size() > 0);
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

void Hierarchy::OnSelectionChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    p_lastSelectedItem = current;
    QTimer::singleShot(100, this, SLOT(_NotifyHierarchyGameObjectSelectionChanged()));
}

void Hierarchy::_NotifyHierarchyGameObjectSelectionChanged()
{
    std::list<GameObject*> selectedGameObjects;
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        if(m_treeItemToGameObject.find(item) != m_treeItemToGameObject.end())
        {
            GameObject *e = m_treeItemToGameObject[item];
            if(e )
                selectedGameObjects.push_back(e);
        }
    }

    /* Expand with one click
    if(selectedGameObjects.size() == 1)
    {
        GameObject *go = selectedGameObjects.front();
        QTreeWidgetItem *item = gameObjectToTreeItem[go];
        item->setExpanded(true);
    }*/

    WindowEventManager::NotifyHierarchyGameObjectsSelected(selectedGameObjects);
}

void Hierarchy::_NotifyHierarchyGameObjectDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(m_treeItemToGameObject.find(item) != m_treeItemToGameObject.end())
    {
        GameObject *selected = m_treeItemToGameObject[item];
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
