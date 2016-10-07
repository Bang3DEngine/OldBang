#ifndef HIERARCHYDRAGDROPMANAGER_H
#define HIERARCHYDRAGDROPMANAGER_H

#include <QTreeWidgetItem>

#include "List.h"
#include "GameObject.h"
#include "IDragDropListener.h"

class Hierarchy;
class HierarchyDragDropManager : public IDragDropListener
{
private:
    Hierarchy *m_hierarchy;

    QTreeWidgetItem *GetDropTargetItem() const;
    GameObject* GetDropTargetGameObject() const;

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDragMove(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    void OnDropHereFromHierarchy(const DragDropInfo &ddi);
    void OnDropHereFromExplorer(const File &f, const DragDropInfo &ddi);

public:
    HierarchyDragDropManager(Hierarchy *hierarchy);
};

#endif // HIERARCHYDRAGDROPMANAGER_H
