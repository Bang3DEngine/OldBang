#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang.h"

#include <QLabel>
#include <QListWidget>

#include "InspectorPartWidget.h"
#include "IWindowEventManagerListener.h"

class Inspector : public QListWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    Entity *currentEntity;

public:
    explicit Inspector(QWidget *parent = 0);

    void Refresh();

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) override;
};

#endif // LISTINSPECTOR_H
