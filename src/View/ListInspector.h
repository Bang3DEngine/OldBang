#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang.h"

#include <QLabel>
#include <QListWidget>

#include "IWindowEventManagerListener.h"

class ListInspector : public QListWidget, public IWindowEventManagerListener
{
    Q_OBJECT

public:
    explicit ListInspector(QWidget *parent = 0);

    void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) override;
};

#endif // LISTINSPECTOR_H
