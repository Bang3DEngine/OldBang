#include "TreeHierarchy.h"
#include "Logger.h"

TreeHierarchy::TreeHierarchy(QWidget *parent)
{

}

TreeHierarchy::~TreeHierarchy()
{
    this->clear();
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
    return eRoot;
}

void TreeHierarchy::Fill(Stage *currentStage)
{
    if(currentStage == nullptr) return;

    entityToTreeItem.clear();
    this->clear();
    this->addTopLevelItem( FillRecursive(currentStage) );
}


void TreeHierarchy::OnChildAdded(Entity *child)
{
    Entity *parent = child->GetParent();
    if(entityToTreeItem.find(parent) != entityToTreeItem.end())
    {
        entityToTreeItem[parent]->addChild( FillRecursive(child) );
    }
    else
    {
        Fill(child->GetStage()); //if the parent isnt found, just redo all the hierarchy
    }
}
