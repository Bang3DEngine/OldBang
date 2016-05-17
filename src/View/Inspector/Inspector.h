#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include <map>

#include "Bang.h"

#include <QLabel>
#include <QListWidget>

#include "InspectorComponentWidget.h"
#include "IWindowEventManagerListener.h"

class Inspector : public QListWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    QLabel *titleLabel = nullptr;
    GameObject *currentGameObject = nullptr;

    std::map<InspectorWidget*, QListWidgetItem*> widgetToItem;

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    explicit Inspector(QWidget *parent = 0);


    void Clear();
    void Refresh();

    void ShowGameObjectInfo(GameObject *gameObject);

    void SetWidget(InspectorWidget *widget);
    void AddWidget(InspectorWidget *widget);

    void MoveUp(InspectorWidget *w);
    void MoveDown(InspectorWidget *w);

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnTreeHierarchyGameObjectsSelected(std::list<GameObject*> &selectedEntities);
};

#endif // LISTINSPECTOR_H
