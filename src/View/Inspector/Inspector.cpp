#include "Inspector.h"

#include <QScrollBar>

#include "GameObject.h"
#include "Component.h"
#include "BehaviourHolder.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SingleLineRenderer.h"
#include "CircleRenderer.h"
#include "Logger.h"


Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
    p_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    clear();
    m_widgetToItem.clear();
    p_currentGameObject = nullptr;
    p_titleLabel->setText(QString::fromStdString("No gameObject selected."));

    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::Refresh()
{
    GameObject *e = p_currentGameObject;
    Clear();
    ShowGameObjectInfo(e);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();
    p_currentGameObject = gameObject;

    NONULL(p_currentGameObject);

    for(Component *p : p_currentGameObject->GetComponents())
    {
        InspectorComponentWidget *w = new InspectorComponentWidget(p);
        AddWidget(w);
    }

    p_titleLabel->setText(
                QString::fromStdString("Name: " + p_currentGameObject->GetName())
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
    m_widgetToItem[widget] = item;
    addItem(item);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());

    adjustSize();
    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::MoveUp(InspectorWidget *w)
{
    int lastRow = row(m_widgetToItem[w]);
    if(lastRow == 0) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow-1, item);
}

void Inspector::MoveDown(InspectorWidget *w)
{
    int lastRow = row(m_widgetToItem[w]);
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
    NONULL(p_currentGameObject);

    if(clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        if(this->p_currentGameObject )
        {
            BehaviourHolder *bh = new BehaviourHolder();
            p_currentGameObject->AddComponent(bh);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentCamera)
    {
        if(this->p_currentGameObject )
        {
            Camera *c = new Camera();
            p_currentGameObject->AddComponent(c);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentTransform)
    {
        if(this->p_currentGameObject )
        {
            Transform *t = new Transform();
            p_currentGameObject->AddComponent(t);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        if(this->p_currentGameObject )
        {
            MeshRenderer *m = new MeshRenderer();
            p_currentGameObject->AddComponent(m);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        if(this->p_currentGameObject )
        {
            SingleLineRenderer *slr = new SingleLineRenderer();
            p_currentGameObject->AddComponent(slr);
        }
    }
    else if(clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        if(this->p_currentGameObject )
        {
            CircleRenderer *cr = new CircleRenderer();
            p_currentGameObject->AddComponent(cr);
        }
    }

    Refresh();
}


