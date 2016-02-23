#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include <QLabel>
#include <QListWidget>

#include "IWindowEventManagerListener.h"

class ListInspector : public QListWidget, public IWindowEventManagerListener
{
    Q_OBJECT

public:
    explicit ListInspector(QWidget *parent = 0);

    void OnTreeHierarchyEntitySelected(Entity *selectedEntity) override;
};

#endif // LISTINSPECTOR_H
