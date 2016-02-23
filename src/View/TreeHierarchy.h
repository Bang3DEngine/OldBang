#ifndef TREEHIERARCHY_H
#define TREEHIERARCHY_H

#include <GL/glew.h>

#include <map>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <QTimer>
#include <QTreeWidget>
#include <QApplication>

#include "Stage.h"
#include "Entity.h"
#include "IWindowEventManagerListener.h"

class TreeHierarchy : public QTreeWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    //For every entity, we have the associated treeItem,
    //in order to update :)
    std::map<Entity*, QTreeWidgetItem*> entityToTreeItem;
    std::map<QTreeWidgetItem*,Entity*> treeItemToEntity;

    void ExpandRecursiveUpwards(QTreeWidgetItem *item);
    QTreeWidgetItem* FillRecursive(Entity *e);

    void UnselectAll();

public:
    explicit TreeHierarchy(QWidget *parent = 0);
    virtual ~TreeHierarchy();

    void Fill(Stage *currentStage);

    void OnChildAdded(Entity *parent) override;

public slots:
    void _NotifyHierarchyItemSelected(QTreeWidgetItem *item, int column);
};

#endif // TREEHIERARCHY_H
