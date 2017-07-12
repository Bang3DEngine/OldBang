#include "Bang/Inspector.h"

#include <QLabel>
#include <QDropEvent>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/BFile.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/SerializableObject.h"
#include "Bang/InspectorWidgetFactory.h"

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
    qRegisterMetaType<IAttributeWidget>("IAttributeWidget");
    Clear();
}

void Inspector::Show(GameObject *gameObject)
{
    Show( InspectorWidgetFactory::CreateWidgets(gameObject) );
}

void Inspector::Show(Component *component)
{
    Show( { InspectorWidgetFactory::CreateWidget(component) } );
}

void Inspector::Show(const BFile &file)
{
    Show( { InspectorWidgetFactory::CreateWidget(file) } );
}

void Inspector::Clear()
{
    for (InspectorWidget *inspWidget : GetCurrentInspectorWidgets())
    {
        inspWidget->OnDestroy();
    }
    m_currentInspectorWidgets.Clear();

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    RefreshSizeHints();

    clear();
}

bool Inspector::Refresh()
{
    return true;
}

void Inspector::Show(const List<InspectorWidget *> &inspWidgets)
{
    Clear();

    for (InspectorWidget *inspWidget : inspWidgets)
    {
        InsertInspectorWidget(inspWidget);
    }

    // m_currentInspectable = inspWidget;

    /*
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
    for (IInspectable *insp : inspectablesToShow) { delete insp; }
    */

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
    // ENSURE(m_currentInspectable);
    // m_currentInspectable->OnEnabledChanged(checked);
}

void Inspector::OnSerializableObjectDestroyed(SerializableObject *destroyed)
{
    /*ENSURE(m_currentInspectable);
    if (destroyed == m_currentInspectable->GetSerializableObject())
    {
        m_currentInspectable = nullptr;
        Clear();
        return;
    }
    */

    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        /*if (iw && iw->GetInspectable()->GetSerializableObject() == destroyed)
        {
            iw->OnDestroy();
            delete itm;
        }*/
    }
}

void Inspector::InsertInspectorWidget(InspectorWidget *inspWidget, int row)
{
    ENSURE(inspWidget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);
    setItemWidget(item, inspWidget);

    m_currentInspectorWidgets.PushBack(inspWidget);
}

Inspector *Inspector::GetInstance()
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->widgetInspector : nullptr;
}

const List<InspectorWidget *> Inspector::GetCurrentInspectorWidgets() const
{
    return m_currentInspectorWidgets;
}

void Inspector::dropEvent(QDropEvent *e)
{
    e->ignore();
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
