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
    QLabel *titleLabel = nullptr;
    Entity *currentEntity = nullptr;

public:
    explicit Inspector(QWidget *parent = 0);

    void Clear();
    void Refresh();

    void ShowEntityInfo(Entity *entity);

    void SetWidget(InspectorWidget *widget);
    void AddWidget(InspectorWidget *widget);

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) override;
};

#endif // LISTINSPECTOR_H
