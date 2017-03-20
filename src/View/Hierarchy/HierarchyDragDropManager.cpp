#include "HierarchyDragDropManager.h"

#include <QTreeWidgetItem>

#include "File.h"
#include "Scene.h"
#include "Debug.h"
#include "Prefab.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"
#include "SceneManager.h"

HierarchyDragDropManager::HierarchyDragDropManager(Hierarchy *hierarchy) :
    p_hierarchy(hierarchy)
{
    p_hierarchy->setAcceptDrops(true);
    p_hierarchy->viewport()->setAcceptDrops(true);
    p_hierarchy->setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    p_hierarchy->setDragEnabled(true);

    p_hierarchy->setDropIndicatorShown(true);
    p_hierarchy->setDefaultDropAction(Qt::DropAction::MoveAction);
    p_hierarchy->setSelectionMode(QAbstractItemView::SingleSelection);
}

QTreeWidgetItem *HierarchyDragDropManager::GetDropTargetItem() const
{
    QPoint pos = p_hierarchy->mapFromGlobal(QCursor::pos());
    return p_hierarchy->itemAt(pos);
}

void HierarchyDragDropManager::GetDropTargetGameObject(
                                            GameObject **dropTargetGameObject,
                                            bool *above, bool *below) const
{
    QTreeWidgetItem *targetItem = GetDropTargetItem();

    QTreeWidget::DropIndicatorPosition dropPos =
            p_hierarchy->dropIndicatorPosition();
    *dropTargetGameObject = p_hierarchy->GetGameObjectFromItem(targetItem);
    *below = (dropPos == QTreeWidget::BelowItem);
    *above = (dropPos == QTreeWidget::AboveItem);
}

void HierarchyDragDropManager::OnDragStart(const DragDropInfo &ddi)
{
    Explorer *explorer = Explorer::GetInstance();
    if (ddi.sourceObject == explorer)
    {
        File f = explorer->GetSelectedFile();
        if (f.IsPrefabAsset())
        {
            // setStyleSheet(IDroppable::acceptDragStyle);
        }
    }
}

void HierarchyDragDropManager::OnDrop(const DragDropInfo &ddi)
{
    Explorer *explorer = Explorer::GetInstance();

    if (ddi.currentObject == p_hierarchy)
    {
        if (ddi.sourceObject == explorer)
        {
            OnDropHereFromExplorer(explorer->GetSelectedFile(), ddi);
        }
        else if (ddi.sourceObject == p_hierarchy)
        {
            if ( AcceptDrop() )
            {
                p_hierarchy->UpdateSceneFromHierarchy();
                p_hierarchy->UpdateHierarchyFromScene();
            }
        }
    }
}

bool HierarchyDragDropManager::AcceptDrop()
{
    GameObject *dropTargetGameObject;
    bool above, below;
    GetDropTargetGameObject(&dropTargetGameObject, &above, &below);
    if (!dropTargetGameObject) { return true; }

    GameObject *selectedGameObject = p_hierarchy->GetFirstSelectedGameObject();
    if (!selectedGameObject) { return true; }

    return (dropTargetGameObject != selectedGameObject) &&
           !dropTargetGameObject->IsChildOf(selectedGameObject);
}

void HierarchyDragDropManager::OnDropHereFromExplorer(const File &f,
                                                      const DragDropInfo &ddi)
{
    ASSERT(!f.GetRelativePath().Empty() > 0, "", return);

    if (f.IsPrefabAsset())
    {
        GameObject *dropTargetGameObject;
        bool above, below;
        GetDropTargetGameObject(&dropTargetGameObject, &above, &below);
        GameObject *aboveGameObject = dropTargetGameObject;
        dropTargetGameObject =
                dropTargetGameObject ? dropTargetGameObject->parent :
                                       SceneManager::GetActiveScene();
        ASSERT(dropTargetGameObject);

        Prefab *prefab = new Prefab();
        prefab->ReadFromFile(f.GetAbsolutePath());
        GameObject *go = prefab->Instantiate(); ASSERT(go);
        go->SetParent(dropTargetGameObject, false, aboveGameObject);
    }
}
