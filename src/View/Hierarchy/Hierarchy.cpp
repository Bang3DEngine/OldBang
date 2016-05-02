#include "Hierarchy.h"

#include "Logger.h"
#include "WindowMain.h"
#include "WindowEventManager.h"

Hierarchy::Hierarchy(QWidget *parent)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(OnItemNameChanged(QTreeWidgetItem*,int)));
}

Hierarchy::~Hierarchy()
{
    this->clear();
}

void Hierarchy::ExpandRecursiveUpwards(QTreeWidgetItem *item)
{
    if(item != nullptr)
    {
        item->setExpanded(true);
        ExpandRecursiveUpwards(item->parent());
    }
}

QTreeWidgetItem* Hierarchy::FillRecursiveDownwards(Entity *e)
{
    const std::list<Entity*> children = e->GetChildren();

    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(e->GetName()));

    for(auto it = children.begin(); it != children.end(); ++it)
    {
        eRoot->addChild( FillRecursiveDownwards( (*it) ) );
    }

    entityToTreeItem[e] = eRoot;
    treeItemToEntity[eRoot] = e;
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
        if(parent != nullptr)
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
}

void Hierarchy::FillDownwards(Stage *currentStage)
{
    if(currentStage == nullptr) return;

    this->currentStage = currentStage;

    entityToTreeItem.clear();
    treeItemToEntity.clear();
    this->clear();

    this->addTopLevelItem( FillRecursiveDownwards(currentStage) );
}

Entity *Hierarchy::GetFirstSelectedEntity() const
{
    if(!selectedItems().empty()) return treeItemToEntity[selectedItems().at(0)];
    return nullptr;
}


void Hierarchy::OnChildAdded(Entity *child)
{
    Entity *parent = child->GetParent();
    if(entityToTreeItem.find(parent) != entityToTreeItem.end())
    {
        //if the parent is found, redo all the childs from the parent
        entityToTreeItem[parent]->addChild( FillRecursiveDownwards(child) );
        ExpandRecursiveUpwards(entityToTreeItem[parent]);

        UnselectAll();
        entityToTreeItem[child]->setSelected(true);
    }
    else
    {
        //if the parent isnt found, just redo all the hierarchy
        FillDownwards(child->GetStage());
    }
}

void Hierarchy::OnChildChangedParent(Entity *child, Entity *previousParent)
{
}

void Hierarchy::OnChildRemoved(Entity *child)
{
    QTreeWidgetItem *item = entityToTreeItem[child];
    if(item != nullptr)
    {
        item->parent()->removeChild(item);
        treeItemToEntity.erase(item);
        entityToTreeItem.erase(child);
    }
}

void Hierarchy::dropEvent(QDropEvent *event)
{
    std::list<QTreeWidgetItem*> sourceItems = selectedItems().toStdList();
    LeaveOnlyTopLevelItems(&sourceItems);

    QTreeWidgetItem *targetItem = itemAt(event->pos());
    if(targetItem != nullptr && !sourceItems.empty())
    {
        Entity *target = treeItemToEntity[targetItem];
        DropIndicatorPosition dropPos = dropIndicatorPosition();
        if (dropPos == BelowItem || dropPos == AboveItem)
        {
            if(target->IsStage())
            {
                return;
            }

            //Not putting inside, but below or above. Thus take its parent
            targetItem = targetItem->parent();
            target = treeItemToEntity[targetItem];
        }

        //Only if the user is not trying to put it on the same level as stage.
        if(targetItem != nullptr) //Trying to put it on the same level as stage. STOP
        {
            for(auto it = sourceItems.begin(); it != sourceItems.end(); ++it)
            {
                QTreeWidgetItem *sourceItem = (*it);
                if(sourceItem != targetItem)
                {
                    Entity *source = treeItemToEntity[sourceItem];
                    if(source != nullptr && target != nullptr &&
                       source->GetParent() != nullptr)
                    {
                        source->GetParent()->MoveChild(source, target);
                    }
                }
            }
        }
    }

    //Only if the user is not trying to put it on the same level as stage.
    if(targetItem != nullptr)
    {
        QTreeWidget::dropEvent(event); //super
        event->acceptProposedAction();
    }
}

void Hierarchy::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if(clickedAction == MenuBar::Action::CreateEmptyEntity)
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
        if(selected != nullptr)
        {
            Qt::ItemFlags oldFlags = selected->flags();
            selected->setFlags(oldFlags | Qt::ItemFlag::ItemIsEditable);

            this->editItem(selected, 0); // Name can be edited now

            //Restore not editable by click
            selected->setFlags(oldFlags);
        }
    }
}

void Hierarchy::OnItemNameChanged(QTreeWidgetItem *item, int column)
{
    if(treeItemToEntity.find(item) != treeItemToEntity.end())
    {   //Change the name of the Entity itself, not just the item text
        treeItemToEntity[item]->SetName(item->text(column).toStdString());
    }
}

void Hierarchy::OnContextMenuCreateEmptyClicked()
{
    Entity *empty = new Entity();
    empty->SetName("Empty");

    foreach(QTreeWidgetItem *item, selectedItems())
    {
        Entity *selected = treeItemToEntity[item];
        selected->AddChild(empty);
    }

    if(selectedItems().size() == 0)
    {
        currentStage->AddChild(empty);
    }
}

void Hierarchy::OnContextMenuDeleteClicked()
{
    std::list<QTreeWidgetItem*> items = selectedItems().toStdList();
    LeaveOnlyTopLevelItems(&items);
    foreach(QTreeWidgetItem *item, items)
    {
        Entity *selected = treeItemToEntity[item];
        if(selected->GetParent() != nullptr)
        {
            selected->GetParent()->RemoveChild(selected);
        }
    }
}

void Hierarchy::OnContextMenuCreatePrefab()
{
    Entity *e = GetFirstSelectedEntity();
    Prefab *prefab = new Prefab(e);

    std::string ext = Prefab::GetFileExtensionStatic();
    QFileDialog fd;
    std::string filename = QFileDialog::getSaveFileName
            (
                WindowMain::GetMainWindow(),
                QString::fromStdString("Create Prefab..."),
                QString::fromStdString(
                    Persistence::GetAssetsPathAbsolute() + "/" + e->GetName()),
                QString::fromStdString( ext + "(*." + ext + ")" )
            )
            .toStdString();
    if(filename == "") return;

    std::fstream f;
    f.open(filename, std::fstream::out);
    if(f.is_open())
    {
        prefab->Write(f);
        f.close();
    }

    delete prefab;
}

void Hierarchy::OnCustomContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = itemAt(point);
    if (item != nullptr)
    {
        Entity *e = treeItemToEntity[item];
        QMenu contextMenu(tr("Entity hierarchy context menu"), this);

        QAction actionCreateEmpty("Create empty", this);
        QAction actionDelete("Delete", this);
        QAction actionCreatePrefab("Create Prefab...", this);

        connect(&actionCreateEmpty, SIGNAL(triggered()), this, SLOT(OnContextMenuCreateEmptyClicked()));
        contextMenu.addAction(&actionCreateEmpty);

        if(!e->IsStage())
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

void Hierarchy::_NotifyHierarchyItemSelectionChanged()
{
    std::list<Entity*> selectedEntities;
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        if(treeItemToEntity.find(item) != treeItemToEntity.end())
        {
            Entity *e = treeItemToEntity[item];
            if(e != nullptr)
                selectedEntities.push_back(e);
        }
    }

    WindowEventManager::NotifyHierarchyEntitiesSelected(selectedEntities);
}
