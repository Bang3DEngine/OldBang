#ifndef HIERARCHYDRAGDROPMANAGER_H
#define HIERARCHYDRAGDROPMANAGER_H

#include "IDragDropListener.h"

class File;
class Hierarchy;
class GameObject;
class HierarchyDragDropManager : public IDragDropListener
{
public:
    HierarchyDragDropManager(Hierarchy *hierarchy);
    bool AcceptDrop();

private:
    Hierarchy *p_hierarchy;

    QTreeWidgetItem *GetDropTargetItem() const;

    /**
     * @brief GetDropTargetGameObject
     * @param parentTargetGameObject = The gameObject where user wants to put the dropping gameObject as child of.
     * @param dropTargetGameObject = The gameObject where the user is dropping the widget over/below/above. This will
     * be the same as parentTargetGameObject only in the case where the user is dropping the dragged gameObject over
     * the gameObject. In case it's being dropped below or above, then they won't be the same.
     */
    void GetDropTargetGameObject(GameObject **dropTargetGameObject,
                                 bool *above, bool *below) const;

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;


    void OnDropHereFromExplorer(const File &f, const DragDropInfo &ddi);

};

#endif // HIERARCHYDRAGDROPMANAGER_H
