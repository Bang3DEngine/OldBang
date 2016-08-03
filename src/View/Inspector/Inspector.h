#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include <map>

#include "Bang.h"

#include <QLabel>
#include <QListWidget>

#include "ComponentWidget.h"
#include "IWindowEventManagerListener.h"

class Inspector : public QListWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    QLabel *m_titleLabel = nullptr;
    GameObject *m_currentGameObject = nullptr;
    InspectorWidget *m_currentInspectorWidget = nullptr;

    std::map<InspectorWidget*, QListWidgetItem*> m_widgetToItem;

    void AddWidget(InspectorWidget *widget);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    explicit Inspector(QWidget *parent = 0);

    void Clear();
    void Refresh();

    void SetInspectable(IInspectable *inspectable, const std::string &title = "");
    void ShowGameObjectInfo(GameObject *gameObject);

    void MoveUp(InspectorWidget *w);
    void MoveDown(InspectorWidget *w);

    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnTreeHierarchyGameObjectsSelected(std::list<GameObject*> &selectedEntities);
};

#endif // LISTINSPECTOR_H
