#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include <QTreeWidgetItem>

class HierarchyItem : public QTreeWidgetItem
{
public:
    HierarchyItem();
    virtual ~HierarchyItem();

    HierarchyItem *Parent() const;
    HierarchyItem *Child(int i) const;
};

#endif // HIERARCHYITEM_H
