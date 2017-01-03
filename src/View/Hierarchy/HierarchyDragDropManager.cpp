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
    m_hierarchy(hierarchy)
{
    m_hierarchy->setAcceptDrops(true);
    m_hierarchy->viewport()->setAcceptDrops(true);
    m_hierarchy->setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    m_hierarchy->setDragEnabled(true);

    m_hierarchy->setDropIndicatorShown(true);
    m_hierarchy->setDefaultDropAction(Qt::DropAction::MoveAction);
    m_hierarchy->setSelectionMode(QAbstractItemView::SingleSelection);
}

QTreeWidgetItem *HierarchyDragDropManager::GetDropTargetItem() const
{
    QPoint pos = m_hierarchy->mapFromGlobal(QCursor::pos());
    return m_hierarchy->itemAt(pos);
}

void HierarchyDragDropManager::GetDropTargetGameObject(GameObject **dropTargetGameObject,
                                                       bool *above, bool *below) const
{
    QTreeWidgetItem *targetItem = GetDropTargetItem();

    QTreeWidget::DropIndicatorPosition dropPos = m_hierarchy->dropIndicatorPosition();
    *dropTargetGameObject = m_hierarchy->GetGameObjectFromItem(targetItem);
    *below = (dropPos == QTreeWidget::BelowItem);
    *above = (dropPos == QTreeWidget::AboveItem);
}

void HierarchyDragDropManager::OnDragStart(const DragDropInfo &ddi)
{
    m_draggingStartGameObject = m_hierarchy->GetFirstSelectedGameObject();

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

    if (ddi.currentObject == m_hierarchy)
    {
        if (ddi.sourceObject == explorer)
        {
            OnDropHereFromExplorer(explorer->GetSelectedFile(), ddi);
        }
        else if (ddi.sourceObject == m_hierarchy)
        {
            OnDropHereFromHierarchy(ddi);
        }
    }
}

void HierarchyDragDropManager::OnDropHereFromHierarchy(const DragDropInfo &ddi)
{
    m_hierarchy->UpdateSceneFromHierarchy();
    m_hierarchy->UpdateHierarchyFromScene();
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
        dropTargetGameObject = dropTargetGameObject ? dropTargetGameObject->parent :
                                                      SceneManager::GetActiveScene();
        ASSERT(dropTargetGameObject);

        Prefab *prefab = new Prefab();
        prefab->ReadXMLInfoFromString(f.GetContents());
        GameObject *go = prefab->Instantiate(); ASSERT(go);
        go->SetParent(dropTargetGameObject, false, aboveGameObject);
    }
}
