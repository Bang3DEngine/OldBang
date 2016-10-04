#include "HierarchyContextMenu.h"

#include "Hierarchy.h"
#include "SceneManager.h"

HierarchyContextMenu::HierarchyContextMenu(Hierarchy *hierarchy)
    : ContextMenu(hierarchy), m_hierarchy(hierarchy)
{
}

void HierarchyContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = m_hierarchy->itemAt(point);
    QMenu contextMenu(tr("GameObject hierarchy context menu"), m_hierarchy);

    QAction  actionCreateEmpty("Create empty",     m_hierarchy);
    QAction         actionCopy("Copy",             m_hierarchy);
    QAction        actionPaste("Paste",            m_hierarchy);
    QAction    actionDuplicate("Duplicate",        m_hierarchy);
    QAction actionCreatePrefab("Create Prefab...", m_hierarchy);
    QAction       actionDelete("Delete",           m_hierarchy);

    actionPaste.setEnabled(GameObjectClipboard::HasSomethingCopied());

    connect(&actionCreateEmpty,  SIGNAL(triggered()), this, SLOT(OnCreateEmptyClicked()));
    connect(&actionCopy,         SIGNAL(triggered()), this, SLOT(OnCopyClicked()));
    connect(&actionPaste,        SIGNAL(triggered()), this, SLOT(OnPasteClicked()));
    connect(&actionDuplicate,    SIGNAL(triggered()), this, SLOT(OnDuplicateClicked()));
    connect(&actionCreatePrefab, SIGNAL(triggered()), this, SLOT(OnCreatePrefab()));
    connect(&actionDelete,       SIGNAL(triggered()), this, SLOT(OnDeleteClicked()));

    if (item)
    {
        contextMenu.addAction(&actionCreateEmpty);
        contextMenu.addAction(&actionCopy);
        contextMenu.addAction(&actionPaste);
        contextMenu.addAction(&actionDuplicate);
        contextMenu.addAction(&actionCreatePrefab);
        contextMenu.addAction(&actionDelete);
    }
    else
    {
        contextMenu.addAction(&actionCreateEmpty);
        contextMenu.addAction(&actionPaste);
    }

    contextMenu.exec(m_hierarchy->mapToGlobal(point));
}

void HierarchyContextMenu::OnCreateEmptyClicked()
{
    GameObject *empty = new GameObject("Empty");

    foreach(QTreeWidgetItem *item, m_hierarchy->selectedItems())
    {
        GameObject *selected = m_hierarchy->GetGameObjectFromItem(item);
        empty->SetParent(selected);
    }

    if (m_hierarchy->selectedItems().empty())
    {
        empty->SetParent(SceneManager::GetActiveScene());
    }

    m_hierarchy->SelectGameObject(empty);
}

void HierarchyContextMenu::OnCopyClicked()
{
    List<GameObject*> whatToCopy = m_hierarchy->GetSelectedGameObjects(true);
    GameObjectClipboard::CopyGameObjects(whatToCopy);
}

void HierarchyContextMenu::OnPasteClicked()
{
    List<GameObject*> selected = m_hierarchy->GetSelectedGameObjects(false);
    List<GameObject*> pasted;
    if (!selected.Empty())
    {
        for (GameObject *sel : selected)
        {
            pasted = GameObjectClipboard::PasteCopiedGameObjectsInto(sel);
        }
    }
    else
    {
        pasted = GameObjectClipboard::PasteCopiedGameObjectsInto(SceneManager::GetActiveScene());
    }

    if (!pasted.Empty())
    {
        m_hierarchy->SelectGameObject(pasted.Front());
    }

}

void HierarchyContextMenu::OnDuplicateClicked()
{
    List<GameObject*> selected = m_hierarchy->GetSelectedGameObjects(true);
    GameObjectClipboard::CopyGameObjects(selected);
    List<GameObject*> duplicated =
            GameObjectClipboard::DuplicateCopiedGameObjects();

    if (!duplicated.Empty())
    {
        m_hierarchy->SelectGameObject(duplicated.Front());
    }
}

void HierarchyContextMenu::OnDeleteClicked()
{
    List<QTreeWidgetItem*> items = m_hierarchy->selectedItems().toStdList();
    m_hierarchy->LeaveOnlyOuterMostItems(&items);
    m_hierarchy->UnselectAll(); // Needed to avoid bug when trying to restore selection
    for (QTreeWidgetItem *item : items)
    {
        GameObject *selected = m_hierarchy->GetGameObjectFromItem(item);
        delete selected;
    }
}

void HierarchyContextMenu::OnCreatePrefab()
{
    GameObject *go = m_hierarchy->GetFirstSelectedGameObject();

    FileDialog fd("Create Prefab...", Prefab::GetFileExtensionStatic());
    String filename = fd.GetSaveFilename(go->name);
    if (filename != "")
    {
        Prefab *prefab = new Prefab(go);
        FileWriter::WriteToFile(filename, prefab);
        delete prefab;
    }
}

