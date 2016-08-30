#include "Inspector.h"

#include <QScrollBar>

#include "Debug.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "GameObject.h"
#include "Application.h"
#include "EditorWindow.h"
#include "MeshRenderer.h"
#include "CircleRenderer.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"
#include "SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent)
    : QListWidget(parent), m_iContextMenu(this)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setAlternatingRowColors(true);
    viewport()->setAcceptDrops(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);

    m_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
    setMinimumWidth(360);
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    if (m_widgetToItem.size() == 0) return; // Avoid double clearings

    clear();

    m_widgetToItem.clear();
    m_currentGameObject = nullptr;
    m_titleLabel->setText(QString::fromStdString(""));

    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        delete iw;
    }
    m_currentInspectorWidgets.clear();
    m_widgetToInspectables.clear();
    m_currentInspectables.clear();
}

void Inspector::Refresh()
{
    if (m_currentGameObject)
    {
        ShowGameObjectInfo(m_currentGameObject);
    }
    else
    {
        if(m_currentInspectables.size() == 1)
        {
            IInspectable *insp = m_currentInspectables[0];
            SetInspectable(insp);
        }
    }
}

void Inspector::SetInspectable(IInspectable *inspectable, const String &title)
{
    Clear();
    InspectorWidget *iw = new InspectorWidget();
    iw->Init(title, inspectable);
    m_widgetToInspectables[iw] = inspectable;
    m_currentInspectables.push_back(inspectable);
    AddWidget(iw);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();

    NONULL(gameObject);
    m_currentGameObject = gameObject;

    for (Component *c : gameObject->GetComponents())
    {
        ComponentWidget *w = new ComponentWidget(c);
        m_currentInspectables.push_back(c);
        m_widgetToInspectables[w] = c;
        AddWidget(w);
    }

    m_titleLabel->setText(gameObject->name.ToQString());
}

void Inspector::ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    m_currentInspectables.push_back(prefabInspectable);
}

void Inspector::RefreshHard()
{
    if (m_currentGameObject)
    {
        ShowGameObjectInfo(m_currentGameObject);
    }
    else
    {
        if(m_currentInspectables.size() == 1)
        {
            IInspectable *insp = m_currentInspectables[0];
            SetInspectable(insp);
        }
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    NONULL(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);

    m_widgetToItem[widget] = item;
    m_currentInspectorWidgets.push_back(widget);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());
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
    return EditorWindow::GetInstance()->widgetInspector;
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    NONULL(m_currentGameObject);

    if (clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        BehaviourHolder *bh = m_currentGameObject->AddComponent<BehaviourHolder>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentCamera)
    {
        Camera *c = m_currentGameObject->AddComponent<Camera>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        MeshRenderer *m = m_currentGameObject->AddComponent<MeshRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        SingleLineRenderer *slr = m_currentGameObject->AddComponent<SingleLineRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        CircleRenderer *cr = m_currentGameObject->AddComponent<CircleRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentDirectionalLight)
    {
        DirectionalLight *dl = m_currentGameObject->AddComponent<DirectionalLight>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentPointLight)
    {
        PointLight *pl = m_currentGameObject->AddComponent<PointLight>();
    }

    Refresh();
}

void Inspector::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (m_currentGameObject == destroyed)
    {
        Clear();
    }
}


