#include "Bang/HierarchyItem.h"

#include "Bang/Hierarchy.h"

HierarchyItem::HierarchyItem()
{
}

HierarchyItem::~HierarchyItem()
{
    Hierarchy::GetInstance()->OnItemDeleted(this);
}

HierarchyItem *HierarchyItem::Parent() const
{
    return static_cast<HierarchyItem*>(parent());
}

HierarchyItem *HierarchyItem::Child(int i) const
{
    return static_cast<HierarchyItem*>(child(i));
}
