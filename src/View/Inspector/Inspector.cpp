#include "Bang/Inspector.h"

#include <QLabel>
#include <QDropEvent>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/SerializableObject.h"

Inspector::Inspector(QWidget *parent) : DragDropQListWidget(),
                                        m_iContextMenu(this)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_titleLabel      = parent->findChild<QLabel*>("labelInspectorName");
    m_enabledCheckBox = parent->findChild<QCheckBox*>("enabledInspectorCheckBox");
    m_enabledCheckBox->setVisible(false);
    setMinimumWidth(300);

    QObject::connect(m_enabledCheckBox, SIGNAL(toggled(bool)),
                     this, SLOT( OnEnabledCheckBoxChanged(bool) ));

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
    while (!m_currentInspectableChildren.Empty())
    {
        SerializableObject *childSerialObject = m_currentInspectableChildren.Front();
        if (childSerialObject->GetInspectorFlags()->IsOn(
                    SerializableObject::InspectorFlag::DeleteWhenCleared))
        {
            delete childSerialObject;
        }
        m_currentInspectableChildren.Remove(childSerialObject);
    }

    List<InspectorWidget*> currentInspWidgets = GetCurrentInspectorWidgets();
    for (InspectorWidget *iw : currentInspWidgets) { iw->OnDestroy(); }

    m_currentInspectable = nullptr;
    m_currentInspectableChildren.Clear();

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    RefreshSizeHints();

    clear();
}

void Inspector::RefreshInspectable(SerializableObject *serializableObject)
{
    if (serializableObject == m_currentInspectable)
    {
        Refresh();
    }
}

void Inspector::Refresh()
{
    ShowInspectable(m_currentInspectable);
}

void Inspector::ShowInspectable(SerializableObject *insp, const String &title)
{
    Clear();

    m_currentInspectable = insp;
    ENSURE(insp);

    typedef SerializableObject::InspectorFlag InspFlag;
    bool showEnabledCheckBox = insp->GetInspectorFlags()->IsOn(
                                          InspFlag::IsEnabledCheckBoxVisible);
    m_enabledCheckBox->setVisible(showEnabledCheckBox);

    bool checkBoxEnabled = insp->GetInspectorFlags()->IsOn(InspFlag::IsEnabled);
    m_enabledCheckBox->setChecked(checkBoxEnabled);

    List<SerializableObject*> serialObjects =
            insp->GetInspectorSerializableObjects();

    for (SerializableObject *serialObject : serialObjects)
    {
        InspectorWidget *iw = serialObject->GetNewInspectorWidget();
        if (iw)
        {
            m_currentInspectableChildren.PushBack(serialObject);
            iw->RefreshWidgetValues();
            AddWidget(iw);
        }
    }

    if (title.Empty())
    {
        String inspTitle = m_currentInspectable->GetTitleInInspector();
        m_titleLabel->setText(inspTitle.ToQString());
    }
    else { m_titleLabel->setText(title.ToQString()); }

    RefreshSizeHints();
}

void Inspector::RefreshSizeHints()
{
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        if (iw)
        {
            itm->setSizeHint( QSize(iw->size().width(),
                                    iw->GetHeightSizeHint()) );
        }
    }
}

void Inspector::OnEnabledCheckBoxChanged(bool checked)
{
    ENSURE(m_currentInspectable);
    m_currentInspectable->OnEnabledChanged(checked);
}

SerializableObject *Inspector::GetCurrentInspectable()
{
    return m_currentInspectable;
}

void Inspector::OnSerializableObjectDestroyed(SerializableObject *destroyed)
{
    if (destroyed == m_currentInspectable)
    {
        m_currentInspectable = nullptr;
        m_currentInspectableChildren.Clear();
        Clear();
        return;
    }

    m_currentInspectableChildren.Remove(destroyed);
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        if (iw && iw->GetRelatedInspectable() == destroyed)
        {
            iw->OnDestroy();
            delete itm;
        }
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    ENSURE(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);
    setItemWidget(item, widget);
}

Inspector *Inspector::GetInstance()
{
    return EditorWindow::GetInstance()->widgetInspector;
}

void Inspector::dropEvent(QDropEvent *e)
{
    e->ignore();
}

List<InspectorWidget*> Inspector::GetCurrentInspectorWidgets() const
{
    List<InspectorWidget*> inspectorWidgets;
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        if (iw) { inspectorWidgets.Add(iw); }
    }
    return inspectorWidgets;
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
