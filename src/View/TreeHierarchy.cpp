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

QTreeWidgetItem* TreeHierarchy::FillRecursive(Entity *e)
{
    const std::list<Entity*> *children = e->GetChildren();
    QTreeWidgetItem *eRoot = new QTreeWidgetItem();
    eRoot->setText(0, QString::fromStdString(e->GetName()) );
    for(auto it = children->begin(); it != children->end(); ++it)
    {
        eRoot->addChild( FillRecursive( (*it) ) );
    }

    entityToTreeItem[e] = eRoot;
    treeItemToEntity[eRoot] = e;
    return eRoot;
}

void TreeHierarchy::UnselectAll()
{
    foreach(QTreeWidgetItem *item, selectedItems())
    {
        item->setSelected(false);
    }
}

void TreeHierarchy::Fill(Stage *currentStage)
{
    if(currentStage == nullptr) return;

    entityToTreeItem.clear();
    treeItemToEntity.clear();
    this->clear();

    this->addTopLevelItem( FillRecursive(currentStage) );
}


void TreeHierarchy::OnChildAdded(Entity *child)
{
    Entity *parent = child->GetParent();
    if(entityToTreeItem.find(parent) != entityToTreeItem.end())
    {
        entityToTreeItem[parent]->addChild( FillRecursive(child) );

        ExpandRecursiveUpwards(entityToTreeItem[parent]);

        UnselectAll();
        entityToTreeItem[child]->setSelected(true);
        WindowEventManager::NotifyHierarchyEntitySelected(child);
    }
    else
    {
        Fill(child->GetStage()); //if the parent isnt found, just redo all the hierarchy
    }
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
    foreach(QTreeWidgetItem *item, selectedItems())
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
        connect(&actionCreateEmpty, SIGNAL(triggered()), this, SLOT(OnContextMenuCreateEmptyClicked()));
        contextMenu.addAction(&actionCreateEmpty);


        if(dynamic_cast<Stage*>(e) == nullptr) //For non-stage entities
        {
            QAction actionDelete("Delete", this);
            connect(&actionDelete, SIGNAL(triggered()), this, SLOT(OnContextMenuDeleteClicked()));
            contextMenu.addAction(&actionDelete);
        }

        contextMenu.exec(mapToGlobal(point));
    }
}

void TreeHierarchy::_NotifyHierarchyItemSelected(QTreeWidgetItem *item, int column)
{
    if(item != nullptr && treeItemToEntity.find(item) != treeItemToEntity.end())
    {
        if(treeItemToEntity[item] != nullptr)
        {
            WindowEventManager::NotifyHierarchyEntitySelected(treeItemToEntity[item]);
        }
    }
}
