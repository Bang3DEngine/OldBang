#include "Bang/Inspector.h"

#include <QLabel>
#include <QDropEvent>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/EditorWindow.h"
#include "Bang/IInspectable.h"
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
    if (m_currentInspectable) { delete m_currentInspectable; }
    m_currentInspectable = nullptr;

    List<InspectorWidget*> currentInspWidgets = GetCurrentInspectorWidgets();
    for (InspectorWidget *iw : currentInspWidgets) { iw->OnDestroy(); }

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    RefreshSizeHints();

    clear();
}

bool Inspector::Refresh(SerializableObject *serialObject)
{
    bool update = m_currentInspectable &&
        (m_currentInspectable->GetSerializableObject() == serialObject);
    if (update)
    {
        Refresh();
    }
    return update;
}

void Inspector::Refresh()
{
    IInspectable *newInsp =
            Object::SCast<IInspectable>(m_currentInspectable->Clone());
    ShowInspectable(newInsp);
}

void Inspector::ShowInspectable(IInspectable *insp)
{
    Clear();

    ENSURE(insp);
    m_currentInspectable = insp;

    String inspTitle = m_currentInspectable->GetTitleInInspector();
    m_titleLabel->setText(inspTitle.ToQString());

    m_enabledCheckBox->setVisible(m_currentInspectable->NeedsEnabledCheckBox());
    m_enabledCheckBox->setChecked(m_currentInspectable->IsEnabled());

    List<IInspectable*> inspectablesToShow =
            m_currentInspectable->GetNewInspectablesToShow();

    for (IInspectable *inspectableToShow : inspectablesToShow)
    {
        InspectorWidget *iw = inspectableToShow->GetNewInspectorWidget();
        if (iw)
        {
            iw->Refresh();
            AddWidget(iw);
        }
    }

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

IInspectable *Inspector::GetCurrentInspectable()
{
    return m_currentInspectable;
}

void Inspector::OnSerializableObjectDestroyed(SerializableObject *destroyed)
{
    ENSURE(m_currentInspectable);
    if (destroyed == m_currentInspectable->GetSerializableObject())
    {
        m_currentInspectable = nullptr;
        Clear();
        return;
    }

    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        if (iw && iw->GetInspectable()->GetSerializableObject() == destroyed)
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
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->widgetInspector : nullptr;
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
