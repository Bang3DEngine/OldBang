#include "Inspector.h"

#include <QScrollBar>

#include "GameObject.h"
#include "Component.h"
#include "BehaviourHolder.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "Transform.h"
#include "Logger.h"


Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
    titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    clear();
    widgetToItem.clear();
    currentGameObject = nullptr;
    titleLabel->setText(QString::fromStdString("No gameObject selected."));

    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::Refresh()
{
    GameObject *e = currentGameObject;
    Clear();
    ShowGameObjectInfo(e);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();
    currentGameObject = gameObject;

    if(currentGameObject == nullptr) return;

    for(Component *p : currentGameObject->GetComponents())
    {
        InspectorComponentWidget *w = new InspectorComponentWidget(p);
        AddWidget(w);
    }

    titleLabel->setText(
                QString::fromStdString("Name: " + currentGameObject->GetName())
                );
}

void Inspector::SetWidget(InspectorWidget *widget)
{
    Clear();
    AddWidget(widget);
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    QListWidgetItem *item = new QListWidgetItem();
    widgetToItem[widget] = item;
    addItem(item);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());

    adjustSize();
    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::MoveUp(InspectorWidget *w)
{
    int lastRow = row(widgetToItem[w]);
    if(lastRow == 0) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow-1, item);
}

void Inspector::MoveDown(InspectorWidget *w)
{
    int lastRow = row(widgetToItem[w]);
    if(lastRow == this->count()) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow+1, item);
}

void Inspector::OnTreeHierarchyGameObjectsSelected
    (std::list<GameObject*> &selectedEntities)
{
    GameObject *e = nullptr;
    if(!selectedEntities.empty())
    {
        e = selectedEntities.front();
    }

    ShowGameObjectInfo(e);
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if(currentGameObject == nullptr) return;

    if(clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        if(this->currentGameObject != nullptr)
        {
            BehaviourHolder *bh = new BehaviourHolder();
            currentGameObject->AddComponent(bh);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentCamera)
    {
        if(this->currentGameObject != nullptr)
        {
            Camera *c = new Camera();
            currentGameObject->AddComponent(c);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        if(this->currentGameObject != nullptr)
        {
            MeshRenderer *m = new MeshRenderer();
            currentGameObject->AddComponent(m);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentLineRenderer)
    {
        if(this->currentGameObject != nullptr)
        {
            LineRenderer *lr = new LineRenderer();
            currentGameObject->AddComponent(lr);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentTransform)
    {
        if(this->currentGameObject != nullptr)
        {
            Transform *t = new Transform();
            currentGameObject->AddComponent(t);
        }
    }

    Refresh();
}


