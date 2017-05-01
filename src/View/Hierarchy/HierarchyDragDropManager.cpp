#include "Bang/HierarchyDragDropManager.h"

#include <QTreeWidgetItem>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"

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
    if (below) { *below = (dropPos == QTreeWidget::DropIndicatorPosition::BelowItem); }
    if (above) { *above = (dropPos == QTreeWidget::DropIndicatorPosition::AboveItem); }
}

void HierarchyDragDropManager::OnDragStart(const DragDropInfo &ddi)
{
    p_selectedItemInDragStart = p_hierarchy->GetFirstSelectedItem();

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
                p_hierarchy->SyncSceneFromHierarchy();
                p_hierarchy->SyncHierarchyFromScene();

                GameObject *selectedGo = p_hierarchy->GetGameObjectFromItem(
                                                     p_selectedItemInDragStart);
                p_hierarchy->SelectGameObject(selectedGo);
            }
        }
    }

    p_selectedItemInDragStart = nullptr;
}

bool HierarchyDragDropManager::AcceptDrop()
{
    if (!p_selectedItemInDragStart) { return false; }

    GameObject *dropTargetGameObject;
    bool above, below;
    GetDropTargetGameObject(&dropTargetGameObject, &above, &below);
    if (!dropTargetGameObject) { return true; }

    GameObject *selectedGameObject =
         p_hierarchy->GetGameObjectFromItem(p_selectedItemInDragStart);
    if (!selectedGameObject) { return true; }

    bool insertingIntoItem = (!above && !below);
    if ( insertingIntoItem && (dropTargetGameObject == selectedGameObject) )
    {
        return false; // Avoid drop into itself
    }

    // Avoid drop into children
    return !dropTargetGameObject->IsChildOf(selectedGameObject);
}

void HierarchyDragDropManager::OnDropHereFromExplorer(const File &f,
                                                      const DragDropInfo &ddi)
{
    if (f.GetPath().GetRelative().Empty()) { return; }

    if (f.IsPrefabAsset())
    {
        GameObject *dropTargetGameObject;
        bool above, below;
        GetDropTargetGameObject(&dropTargetGameObject, &above, &below);
        GameObject *aboveGameObject = dropTargetGameObject;
        dropTargetGameObject =
                dropTargetGameObject ? dropTargetGameObject->parent :
                                       SceneManager::GetActiveScene();
        ENSURE(dropTargetGameObject);

        Prefab *prefab = new Prefab();
        prefab->ReadFromFile(f.GetPath());
        GameObject *go = prefab->Instantiate(); ENSURE(go);
        go->SetParent(dropTargetGameObject, false, aboveGameObject);
    }
}
