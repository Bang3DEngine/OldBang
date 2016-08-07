#include "Inspector.h"

#include <QScrollBar>

#include "Logger.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "WindowMain.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "CircleRenderer.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"
#include "SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);

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
    m_titleLabel->setText(QString::fromStdString(""));

    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        delete iw;
    }
    m_currentInspectorWidgets.clear();
    m_currentInspectables.clear();

    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::Refresh()
{
    if (m_currentGameObject)
    {
        Clear();
        ShowGameObjectInfo(m_currentGameObject);
    }
}

void Inspector::SetInspectable(IInspectable *inspectable, const std::string &title)
{
    Clear();
    m_currentInspectorWidgets.push_back(new InspectorWidget(title, inspectable));
    m_currentInspectables.push_back(inspectable);
    AddWidget(m_currentInspectorWidgets[0]);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();

    NONULL(gameObject);
    m_currentGameObject = gameObject;

    for (Component *c : gameObject->GetComponents())
    {
        ComponentWidget *w = new ComponentWidget(c);
        m_currentInspectorWidgets.push_back(w);
        m_currentInspectables.push_back(c);
        AddWidget(w);
    }

    m_titleLabel->setText(QString(gameObject->name.c_str()));
}

void Inspector::ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    // We need this line to make IsShowingInspectable work with
    // this prefab fileInspectable
    m_currentInspectables.push_back(prefabInspectable);
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    NONULL(widget);

    QListWidgetItem *item = new QListWidgetItem();
    m_widgetToItem[widget] = item;
    addItem(item);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size()); // error aqui
    widget->adjustSize();
    widget->show();

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

std::vector<IInspectable *> Inspector::GetCurrentInspectables() const
{
    return m_currentInspectables;
}

bool Inspector::IsShowingInspectable(IInspectable *inspectable) const
{
    for (IInspectable *insp : m_currentInspectables)
    {
        if (inspectable == insp)
        {
            return true;
        }
    }
    return false;
}

Inspector *Inspector::GetInstance()
{
    return WindowMain::GetInstance()->widgetInspector;
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    NONULL(m_currentGameObject);

    if (clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        BehaviourHolder *bh = new BehaviourHolder();
        m_currentGameObject->AddComponent(bh);
    }
    else if (clickedAction == MenuBar::Action::AddComponentCamera)
    {
        Camera *c = new Camera();
        m_currentGameObject->AddComponent(c);
    }
    else if (clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        MeshRenderer *m = new MeshRenderer();
        m_currentGameObject->AddComponent(m);
    }
    else if (clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        SingleLineRenderer *slr = new SingleLineRenderer();
        m_currentGameObject->AddComponent(slr);
    }
    else if (clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        CircleRenderer *cr = new CircleRenderer();
        m_currentGameObject->AddComponent(cr);
    }
    else if (clickedAction == MenuBar::Action::AddComponentDirectionalLight)
    {
        DirectionalLight *dl = new DirectionalLight();
        m_currentGameObject->AddComponent(dl);
    }
    else if (clickedAction == MenuBar::Action::AddComponentPointLight)
    {
        PointLight *pl = new PointLight();
        m_currentGameObject->AddComponent(pl);
    }

    Refresh();
}


