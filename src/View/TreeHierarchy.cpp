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
        Logger_Log("a");
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

void TreeHierarchy::_NotifyHierarchyItemSelected(QTreeWidgetItem *item, int column)
{
    WindowEventManager::NotifyHierarchyEntitySelected(treeItemToEntity[item]);
}
