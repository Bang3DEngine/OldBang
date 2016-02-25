#include "TreeHierarchy.h"

#include "WindowEventManager.h"
#include "Logger.h"

TreeHierarchy::TreeHierarchy(QWidget *parent)
{

}

TreeHierarchy::~TreeHierarchy()
{
    this->clear();
}

void TreeHierarchy::ExpandRecursiveUpwards(QTreeWidgetItem *item)
{
    if(item != nullptr)
    {
        item->setExpanded(true);
        ExpandRecursiveUpwards(item->parent());
    }
}

QTreeWidgetItem* TreeHierarchy::FillRecursiveDownwards(Entity *e)
{
    const std::list<Entity*> children = e->GetChildren();
    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    std::ostringstream log;
    log << ((void*)e);
    //e->SetName(log.str());
    eRoot->setText(0, QString::fromStdString(e->GetName()));
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        eRoot->addChild( FillRecursiveDownwards( (*it) ) );
    }

    entityToTreeItem[e] = eRoot;
    treeItemToEntity[eRoot] = e;
    return eRoot;
}

void TreeHierarchy::LeaveOnlyTopLevelItems(std::list<QTreeWidgetItem*> *items)
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

void TreeHierarchy::UnselectAll()
{
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        item->setSelected(false);
    }
}

void TreeHierarchy::FillDownwards(Stage *currentStage)
{
    if(currentStage == nullptr) return;

    entityToTreeItem.clear();
    treeItemToEntity.clear();
    this->clear();

    this->addTopLevelItem( FillRecursiveDownwards(currentStage) );
}

Entity *TreeHierarchy::GetFirstSelectedEntity() const
{
    if(!selectedItems().empty()) return treeItemToEntity[selectedItems().at(0)];
    return nullptr;
}


void TreeHierarchy::OnChildAdded(Entity *child)
{
    Entity *parent = child->GetParent();
    if(entityToTreeItem.find(parent) != entityToTreeItem.end())
    {
        entityToTreeItem[parent]->addChild( FillRecursiveDownwards(child) );
        ExpandRecursiveUpwards(entityToTreeItem[parent]);

        UnselectAll();
        entityToTreeItem[child]->setSelected(true);
    }
    else
    {
        FillDownwards(child->GetStage()); //if the parent isnt found, just redo all the hierarchy
    }
}

void TreeHierarchy::OnChildChangedParent(Entity *child, Entity *previousParent)
{
}

void TreeHierarchy::OnChildRemoved(Entity *child)
{
    QTreeWidgetItem *item = entityToTreeItem[child];
    if(item != nullptr)
    {
        item->parent()->removeChild(item);
        treeItemToEntity.erase(item);
        entityToTreeItem.erase(child);
    }
}

void TreeHierarchy::dropEvent(QDropEvent *event)
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

void TreeHierarchy::OnContextMenuCreateEmptyClicked()
{
    Entity *empty = new Entity();
    empty->SetName("Empty");

    foreach(QTreeWidgetItem *item, selectedItems())
    {
        Entity *selected = treeItemToEntity[item];
        selected->AddChild(empty);
    }
}

void TreeHierarchy::OnContextMenuDeleteClicked()
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

void TreeHierarchy::OnCustomContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = itemAt(point);
    if (item != nullptr)
    {
        Entity *e = treeItemToEntity[item];
        QMenu contextMenu(tr("Entity hierarchy context menu"), this);

        QAction actionCreateEmpty("Create empty", this);
        QAction actionDelete("Delete", this);

        connect(&actionCreateEmpty, SIGNAL(triggered()), this, SLOT(OnContextMenuCreateEmptyClicked()));
        contextMenu.addAction(&actionCreateEmpty);

        if(!e->IsStage())
        {
            connect(&actionDelete, SIGNAL(triggered()), this, SLOT(OnContextMenuDeleteClicked()));
            contextMenu.addAction(&actionDelete);
        }

        contextMenu.exec(mapToGlobal(point));
    }
}

void TreeHierarchy::_NotifyHierarchyItemSelectionChanged()
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
