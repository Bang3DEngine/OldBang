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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Logger.h"


Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
    m_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
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
    m_currentGameObject = nullptr;
    m_titleLabel->setText(QString::fromStdString("No gameObject selected."));

    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::Refresh()
{
    GameObject *e = m_currentGameObject;
    Clear();
    ShowGameObjectInfo(e);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();
    m_currentGameObject = gameObject;

    NONULL(m_currentGameObject);

    for (Component *p : m_currentGameObject->GetComponents())
    {
        InspectorComponentWidget *w = new InspectorComponentWidget(p);
        AddWidget(w);
    }

    m_titleLabel->setText(
                QString::fromStdString("Name: " + m_currentGameObject->name)
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
    if (lastRow == 0) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow-1, item);
}

void Inspector::MoveDown(InspectorWidget *w)
{
    int lastRow = row(m_widgetToItem[w]);
    if (lastRow == this->count()) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow+1, item);
}

void Inspector::OnTreeHierarchyGameObjectsSelected
    (std::list<GameObject*> &selectedEntities)
{
    GameObject *e = nullptr;
    if (!selectedEntities.empty())
    {
        e = selectedEntities.front();
    }

    ShowGameObjectInfo(e);
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    NONULL(m_currentGameObject);

    if (clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        if (this->m_currentGameObject )
        {
            BehaviourHolder *bh = new BehaviourHolder();
            m_currentGameObject->AddComponent(bh);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentCamera)
    {
        if (this->m_currentGameObject )
        {
            Camera *c = new Camera();
            m_currentGameObject->AddComponent(c);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentTransform)
    {
        if (this->m_currentGameObject )
        {
            Transform *t = new Transform();
            m_currentGameObject->AddComponent(t);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        if (this->m_currentGameObject )
        {
            MeshRenderer *m = new MeshRenderer();
            m_currentGameObject->AddComponent(m);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        if (this->m_currentGameObject )
        {
            SingleLineRenderer *slr = new SingleLineRenderer();
            m_currentGameObject->AddComponent(slr);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        if (this->m_currentGameObject )
        {
            CircleRenderer *cr = new CircleRenderer();
            m_currentGameObject->AddComponent(cr);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentDirectionalLight)
    {
        if (this->m_currentGameObject )
        {
            DirectionalLight *dl = new DirectionalLight();
            m_currentGameObject->AddComponent(dl);
        }
    }
    else if (clickedAction == MenuBar::Action::AddComponentPointLight)
    {
        if (this->m_currentGameObject )
        {
            PointLight *pl = new PointLight();
            m_currentGameObject->AddComponent(pl);
        }
    }

    Refresh();
}


