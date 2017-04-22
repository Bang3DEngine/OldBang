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
    //m_enableGameObjectCheckBox =
    //        parent->findChild<QCheckBox*>("enableGameObjectCheckBox");
    //m_enableGameObjectCheckBox->setVisible(false);
    setMinimumWidth(300);

    //QObject::connect(m_enableGameObjectCheckBox, SIGNAL(toggled(bool)),
    //                 this, SLOT( OnEnableGameObjectCheckBoxChanged(bool) ));

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

    while (!m_currentSerialObjects.Empty())
    {
        SerializableObject *serialObject = m_currentSerialObjects.Front();
        if (serialObject->GetInspectorFlags()->IsOn(
                    SerializableObject::InspectorFlag::DeleteWhenCleared))
        {
            delete serialObject; // This will remove it from the list
        }
        else
        {
            m_currentSerialObjects.Remove(serialObject);
        }
    }
    for (InspectorWidget *iw : m_currentInspectorWidgets) { delete iw; }


    m_currentInspectorWidgets.Clear();
    m_widget_To_Inspectables.Clear();
    m_currentSerialObjects.Clear();
    m_widget_To_Item.Clear();

    m_titleLabel->setText(tr(""));
    //m_enableGameObjectCheckBox->setVisible(false);

    clear();
}

void Inspector::Refresh()
{
    ENSURE(!m_currentSerialObjects.Empty());
    if(m_currentSerialObjects.Size() >= 1)
    {
        SerializableObject *insp = m_currentSerialObjects.Front();
        ShowInspectable(insp);
    }
    RefreshSizeHints();
}

void Inspector::ShowInspectable(SerializableObject *inspectable,
                                const String &title)
{
    Clear();
    List<SerializableObject*> serialObjects =
            inspectable->GetInspectorSerializableObjects();

    for (SerializableObject *serialObject : serialObjects)
    {
        InspectorWidget *iw = serialObject->GetNewInspectorWidget();
        if (iw)
        {
            m_widget_To_Inspectables[iw] = serialObject;
            m_currentSerialObjects.PushBack(serialObject);

            iw->RefreshWidgetValues();
            AddWidget(iw);
        }
    }
    RefreshSizeHints();
    //m_titleLabel->setText(gameObject->name.ToQString());
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

/*
void Inspector::OnEnableGameObjectCheckBoxChanged(bool checked)
{
    if (p_currentGameObject)
    {
        p_currentGameObject->SetEnabled(checked);
    }
}
*/

SerializableObject *Inspector::GetFirstSerializableObject()
{
    return !m_currentSerialObjects.Empty() ? m_currentSerialObjects.Front() :
                                             nullptr;
}

void Inspector::OnSerializableObjectDestroyed(SerializableObject *destroyed)
{
    bool mustRefresh = false;

    if (m_currentSerialObjects.Contains(destroyed))
    {
        m_currentSerialObjects.Remove(destroyed);
        mustRefresh = true;
    }

    List<InspectorWidget*> widgetsToRemove =
            m_widget_To_Inspectables.GetKeysWithValue(destroyed);
    for (InspectorWidget *inspectorWidget : widgetsToRemove)
    {
        QListWidgetItem *item = m_widget_To_Item[inspectorWidget];
        removeItemWidget(item);
        m_widget_To_Inspectables.Remove(inspectorWidget);
        m_widget_To_Item.Remove(inspectorWidget);
        m_currentInspectorWidgets.Remove(inspectorWidget);
        delete inspectorWidget;
    }

    if (mustRefresh)
    {
        Refresh();
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
    return m_currentSerialObjects.Contains(inspectable);
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



