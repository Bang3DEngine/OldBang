#include "Bang/Inspector.h"

#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Chrono.h"
#include "Bang/UIText.h"
#include "Bang/Canvas.h"
#include "Bang/Camera.h"
#include "Bang/UIImage.h"
#include "Bang/Component.h"
#include "Bang/Transform.h"
#include "Bang/Behaviour.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/Application.h"
#include "Bang/EditorWindow.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/AudioListener.h"
#include "Bang/CircleRenderer.h"
#include "Bang/ComponentWidget.h"
#include "Bang/DirectionalLight.h"
#include "Bang/SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent)
    : DragDropQListWidget(), m_iContextMenu(this)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
    m_enableGameObjectCheckBox =
            parent->findChild<QCheckBox*>("enableGameObjectCheckBox");
    setMinimumWidth(300);

    QObject::connect(m_enableGameObjectCheckBox, SIGNAL(toggled(bool)),
                     this, SLOT( OnEnableGameObjectCheckBoxChanged(bool) ));

    horizontalScrollBar()->setEnabled(false);
}

void Inspector::OnWindowShown()
{
    this->SetDragDropEventPossibleSources({
          Explorer::GetInstance(), Hierarchy::GetInstance()});
    Clear();
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    ENSURE(!m_widget_To_Item.Empty());

    clear();
    for (InspectorWidget *iw : m_currentInspectorWidgets) { iw->OnDestroy(); }
    m_currentInspectorWidgets.Clear();
    m_widget_To_Inspectables.Clear();
    m_currentInspectables.Clear();
    m_widget_To_Item.Clear();

    m_titleLabel->setText(tr(""));
    p_currentGameObject = nullptr;
    m_enableGameObjectCheckBox->setVisible(false);
}

void Inspector::Refresh()
{
    if (p_currentGameObject)
    {
        ShowGameObjectInfo(p_currentGameObject);
    }
    else if(m_currentInspectables.Size() == 1)
    {
        SerializableObject *insp = m_currentInspectables.Front();
        SetInspectable(insp);
    }
    RefreshSizeHints();
}

void Inspector::SetInspectable(SerializableObject *inspectable,
                               const String &title)
{
    Clear();
    InspectorWidget *iw = new InspectorWidget();
    iw->Init(title, inspectable);
    m_widget_To_Inspectables[iw] = inspectable;
    m_currentInspectables.PushBack(inspectable);
    AddWidget(iw);
    RefreshSizeHints();
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Chrono ch;
    ch.MarkEvent("Clear");
    Clear();

    ENSURE(gameObject);
    p_currentGameObject = gameObject;

    for (Component *c : gameObject->GetComponents())
    {
        String str = "Create ComponentWidget for "; str += c->ToString();
        ch.MarkEvent(str);
        ComponentWidget *w = new ComponentWidget(c);
        m_currentInspectables.PushBack(c);
        m_widget_To_Inspectables[w] = c;
        w->RefreshWidgetValues();
        AddWidget(w);
    }

    ch.MarkEvent("Refresh Size hints");
    RefreshSizeHints();
    m_titleLabel->setText(gameObject->name.ToQString());
    m_enableGameObjectCheckBox->setVisible(true);
    m_enableGameObjectCheckBox->setChecked( p_currentGameObject->IsEnabled() );
    ch.Log();
}

void Inspector::RefreshSizeHints()
{
    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        QListWidgetItem *item = m_widget_To_Item[iw];
        int width = iw->size().width();
        const int height = iw->GetHeightSizeHint();
        item->setSizeHint( QSize(width, height) );
    }
}

void Inspector::OnEditorPlay()
{
    Clear();
}

void Inspector::OnEnableGameObjectCheckBoxChanged(bool checked)
{
    if (p_currentGameObject)
    {
        p_currentGameObject->SetEnabled(checked);
    }
}

void Inspector::ShowPrefabInspectableInfo(
        PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    m_currentInspectables.PushBack(prefabInspectable);
}

void Inspector::OnMenuBarAddNewBehaviourClicked()
{
    ENSURE(p_currentGameObject);
    Behaviour *newBehaviour = Behaviour::CreateNewBehaviour();
    if (newBehaviour)
    {
        p_currentGameObject->AddComponent(newBehaviour);
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    ENSURE(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);

    m_widget_To_Item[widget] = item;
    m_currentInspectorWidgets.PushBack(widget);

    setItemWidget(item, widget);
}

bool Inspector::IsShowingInspectable(SerializableObject *inspectable) const
{
    return m_currentInspectables.Contains(inspectable);
}

Inspector *Inspector::GetInstance()
{
    return EditorWindow::GetInstance()->widgetInspector;
}

void Inspector::dropEvent(QDropEvent *e)
{
    e->ignore();
}


String Inspector::FormatInspectorLabel(const String &labelString)
{
    String labelFormatted = labelString.Replace("_", " ");

    // Add space after every cap (except for consecutive caps).
    for (auto it = labelFormatted.Begin(); it != labelFormatted.End(); ++it)
    {
        ++it;
        bool nextOneIsCap = (it != labelFormatted.End()) &&
                             String::IsUpperCase(*it);
        --it;
        if (!nextOneIsCap && String::IsUpperCase(*it) &&
            it != labelFormatted.Begin())
        {
            it = labelFormatted.insert(it, ' ');
            ++it;
        }
    }

    labelFormatted = labelFormatted;
    return labelFormatted.Replace("  ", " ");
}


void Inspector::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (p_currentGameObject == destroyed)
    {
        Clear();
    }
}


