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
    if(item )
    {
        item->setExpanded(true);
        ExpandRecursiveUpwards(item->parent());
    }
}

QTreeWidgetItem* Hierarchy::FillDownwards(GameObject *o)
{
    if(o->IsEditorGameObject() && false)
    {   //The scene is an editor object, but must pass this point
        if(dynamic_cast<EditorScene*>(o) == nullptr) return nullptr;
    }

    const std::list<GameObject*> children = o->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(o->GetName()));

    for(auto it = children.begin(); it != children.end(); ++it)
    {
        QTreeWidgetItem *c = FillDownwards(*it);
        if(c )
        {
            eRoot->addChild(c);
        }
    }

    gameObjectToTreeItem[o] = eRoot;
    treeItemToGameObject[eRoot] = o;
    return eRoot;
}

void Hierarchy::LeaveOnlyTopLevelItems(std::list<QTreeWidgetItem*> *items)
{
    //For each item, it will be a top level item,
    //if non of the selected items is its parent
    std::list<QTreeWidgetItem*> result;
    for(auto it = items->begin(); it != items->end(); ++it)
    {
        QTreeWidgetItem *parent = (*it)->parent();
        if(parent )
        {
            bool hasItsParentInTheList = false;
            for(auto it2 = items->begin(); it2 != items->end(); ++it2)
            {
                if(parent == (*it2))
                {
                    hasItsParentInTheList = true;
                    break;
                }
            }

            if(!hasItsParentInTheList)
            {
                result.push_back((*it));
            }
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

void Hierarchy::Refresh(Scene *currentScene)
{
    NONULL(currentScene);

    this->currentScene = currentScene;
    gameObjectToTreeItem.clear();
    treeItemToGameObject.clear();
    this->clear();
    this->addTopLevelItem( FillDownwards(currentScene) );
}

GameObject *Hierarchy::GetFirstSelectedGameObject() const
{
    if(!selectedItems().empty()) return treeItemToGameObject[selectedItems().at(0)];
    return nullptr;
}


void Hierarchy::OnChildAdded(GameObject *child)
{
    GameObject *parent = child->GetParent();
    if(gameObjectToTreeItem.find(parent) != gameObjectToTreeItem.end())
    {
        //if the parent is found, redo all the childs from the parent
        QTreeWidgetItem *item = FillDownwards(child); NONULL(item);

        gameObjectToTreeItem[parent]->addChild(item);
        ExpandRecursiveUpwards(gameObjectToTreeItem[parent]);

        if(!child->IsEditorGameObject())
        {
            UnselectAll();
            gameObjectToTreeItem[child]->setSelected(true);
        }
    }
    else
    {
        //if the parent isnt found, just redo all the hierarchy
        Refresh(child->GetScene());
    }
}

void Hierarchy::OnChildChangedParent(GameObject *child, GameObject *previousParent)
{
}

void Hierarchy::OnChildRemoved(GameObject *child)
{
    QTreeWidgetItem *item = gameObjectToTreeItem[child];
    if(item )
    {
        item->parent()->removeChild(item);
        treeItemToGameObject.erase(item);
        gameObjectToTreeItem.erase(child);
    }
}

void Hierarchy::dropEvent(QDropEvent *event)
{
    std::list<QTreeWidgetItem*> sourceItems = selectedItems().toStdList();
    LeaveOnlyTopLevelItems(&sourceItems);

    QTreeWidgetItem *targetItem = itemAt(event->pos());
    if(targetItem  && !sourceItems.empty())
    {
        GameObject *target = treeItemToGameObject[targetItem];
        DropIndicatorPosition dropPos = dropIndicatorPosition();
        if (dropPos == BelowItem || dropPos == AboveItem)
        {
            if(target->IsScene())
            {
                return;
            }

            //Not putting inside, but below or above. Thus take its parent
            targetItem = targetItem->parent();
            target = treeItemToGameObject[targetItem];
        }

        //Only if the user is not trying to put it on the same level as scene.
        if(targetItem ) //Trying to put it on the same level as scene. STOP
        {
            for(auto it = sourceItems.begin(); it != sourceItems.end(); ++it)
            {
                QTreeWidgetItem *sourceItem = (*it);
                if(sourceItem != targetItem)
                {
                    GameObject *source = treeItemToGameObject[sourceItem];
                    if(source  && target  &&
                       source->GetParent() )
                    {
                        source->GetParent()->MoveChild(source, target);
                    }
                }
            }
        }
    }

    //Only if the user is not trying to put it on the same level as scene.
    if(targetItem )
    {
        QTreeWidget::dropEvent(event); //super
        event->acceptProposedAction();
    }
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


    if(gameObjectToTreeItem.find(go) != gameObjectToTreeItem.end())
    {
        GameObject *parent = go->GetParent();
        if(parent )
        {
            ExpandRecursiveUpwards(gameObjectToTreeItem[parent]);
        }

        UnselectAll();
        gameObjectToTreeItem[go]->setSelected(true);
        _NotifyHierarchyGameObjectSelectionChanged();
    }
}

void Hierarchy::OnItemNameChanged(QTreeWidgetItem *item, int column)
{
    if(treeItemToGameObject.find(item) != treeItemToGameObject.end())
    {   //Change the name of the GameObject itself, not just the item text
        treeItemToGameObject[item]->SetName(item->text(column).toStdString());
    }
}

void Hierarchy::OnContextMenuCreateEmptyClicked()
{
    GameObject *empty = new GameObject();
    empty->SetName("Empty");

    foreach(QTreeWidgetItem *item, selectedItems())
    {
        GameObject *selected = treeItemToGameObject[item];
        selected->AddChild(empty);
    }

    if(selectedItems().size() == 0)
    {
        currentScene->AddChild(empty);
    }
}

void Hierarchy::OnContextMenuDeleteClicked()
{
    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    LeaveOnlyTopLevelItems(&items);
    foreach(QTreeWidgetItem *item, items)
    {
        GameObject *selected = treeItemToGameObject[item];
        if(selected->GetParent() )
        {
            selected->GetParent()->RemoveChild(selected);
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
    if (item )
    {
        GameObject *e = treeItemToGameObject[item];
        QMenu contextMenu(tr("GameObject hierarchy context menu"), this);

        QAction actionCreateEmpty("Create empty", this);
        QAction actionDelete("Delete", this);
        QAction actionCreatePrefab("Create Prefab...", this);

        connect(&actionCreateEmpty, SIGNAL(triggered()), this, SLOT(OnContextMenuCreateEmptyClicked()));
        contextMenu.addAction(&actionCreateEmpty);

        if(!e->IsScene())
        {
            connect(&actionDelete, SIGNAL(triggered()), this, SLOT(OnContextMenuDeleteClicked()));
            contextMenu.addAction(&actionDelete);

            if(selectedItems().length() == 1)
            {
                connect(&actionCreatePrefab, SIGNAL(triggered()), this, SLOT(OnContextMenuCreatePrefab()));
                contextMenu.addAction(&actionCreatePrefab);
            }
        }

        contextMenu.exec(mapToGlobal(point));
    }
}

void Hierarchy::OnSelectionChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QTimer::singleShot(100, this, SLOT(_NotifyHierarchyGameObjectSelectionChanged()));
}

void Hierarchy::_NotifyHierarchyGameObjectSelectionChanged()
{
    std::list<GameObject*> selectedGameObjects;
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        if(treeItemToGameObject.find(item) != treeItemToGameObject.end())
        {
            GameObject *e = treeItemToGameObject[item];
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
    if(treeItemToGameObject.find(item) != treeItemToGameObject.end())
    {
        GameObject *selected = treeItemToGameObject[item];
        WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(selected);
    }
}
