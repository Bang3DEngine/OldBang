#include "HierarchyDragDropManager.h"

#include "Hierarchy.h"
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

GameObject *HierarchyDragDropManager::GetDropTargetGameObject() const
{
    QTreeWidgetItem *targetItem = GetDropTargetItem();
    GameObject *targetGameObject = nullptr;
    if (targetItem)
    {
        targetGameObject = m_hierarchy->GetGameObjectFromItem(targetItem);
        QTreeWidget::DropIndicatorPosition dropPos =
                m_hierarchy->dropIndicatorPosition();
        if (dropPos == QTreeWidget::BelowItem ||
            dropPos == QTreeWidget::AboveItem)
        {
            //Not putting inside, but below or above. Thus take its parent.
            targetItem = targetItem->parent();
            if (targetItem)
            {
                targetGameObject =
                    m_hierarchy->GetGameObjectFromItem(targetItem);
            }
            else
            {
                targetGameObject = SceneManager::GetActiveScene();
            }
        }
    }
    else
    {
        targetGameObject = SceneManager::GetActiveScene();
    }

    return targetGameObject;
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

void HierarchyDragDropManager::OnDragMove(const DragDropInfo &ddi)
{
    QTreeWidgetItem *item = GetDropTargetItem();

    if (item)
    {
        item->setExpanded(true);
    }
}

void HierarchyDragDropManager::OnDrop(const DragDropInfo &ddi)
{
    if ( IDragDropListener::MouseOver(m_hierarchy) )
    {
        Explorer *explorer = Explorer::GetInstance();
        if (ddi.sourceObject == explorer)
        {
            OnDropHereFromExplorer(explorer->GetSelectedFile(), ddi);
        }
        else if (ddi.sourceObject == m_hierarchy)
        {
            OnDropHereFromHierarchy(m_hierarchy->GetFirstSelectedGameObject(),
                                    ddi);
        }
    }

    // setStyleSheet("/* */");
}

void HierarchyDragDropManager::OnDropHereFromHierarchy(GameObject *selected,
                                                       const DragDropInfo &ddi)
{
    QEvent *e = ddi.currentEvent;

    GameObject *targetGameObject = GetDropTargetGameObject();
    List<QTreeWidgetItem*> sourceItems =
            m_hierarchy->selectedItems().toStdList();
    m_hierarchy->LeaveOnlyOuterMostItems(&sourceItems);
    if (!sourceItems.Empty())
    {
        QTreeWidgetItem *targetItem = GetDropTargetItem();
        for (QTreeWidgetItem *sourceItem : sourceItems)
        {
            if (sourceItem != targetItem)
            {
                GameObject *sourceGameObject =
                        m_hierarchy->GetGameObjectFromItem(sourceItem);
                if (sourceGameObject != targetGameObject &&
                    sourceGameObject && sourceGameObject->parent &&
                    !targetGameObject->IsChildOf(sourceGameObject))
                {
                    sourceGameObject->SetParent(targetGameObject, true);
                    e->accept();
                }
            }
        }
    }
    m_hierarchy->RefreshFromScene();
}

void HierarchyDragDropManager::OnDropHereFromExplorer(const File &f,
                                                      const DragDropInfo &ddi)
{
    QDropEvent *e = static_cast<QDropEvent*>(ddi.currentEvent);
    if (f.GetRelativePath().length() == 0) { e->ignore(); return; }

    if (f.IsPrefabAsset())
    {
        GameObject *targetGameObject = GetDropTargetGameObject();
        Prefab *prefab = new Prefab();
        prefab->ReadXMLInfoFromString(f.GetContents());
        GameObject *go = prefab->Instantiate();
        go->SetParent(targetGameObject);
        e->accept();
        return;
    }

    e->ignore();
}
