#include "Bang/AttrWidgetFile.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/File.h"
#include "Bang/Dialog.h"
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/Application.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/DialogBrowseAssetFile.h"

AttrWidgetFile::AttrWidgetFile(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    m_fileExtension = xmlAttribute.GetPropertyValue(
                        XMLProperty::FileExtension.GetName());

    m_hLayout = new QHBoxLayout();
    m_layout.addLayout(m_hLayout, 1);

    // Icon
    String filepath = xmlAttribute.GetValue();
    bool isEngineFile = xmlAttribute.HasProperty(XMLProperty::IsEngineFile);
    filepath = IO::ToAbsolute(filepath, isEngineFile);
    //

    m_iconLabel = new QLabel();
    m_iconLabel->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_hLayout->addWidget(m_iconLabel, 0, Qt::AlignRight | Qt::AlignVCenter);

    // File Line Edit
    m_filepathLineEdit = new FileLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    m_filepathLineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_filepathLineEdit->setMinimumWidth(50);
    m_filepathLineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_hLayout->addWidget(m_filepathLineEdit, 100);
    //

    // Browse button
    static QIcon browseIcon(":/qss_icons/rc/radio_checked.png");
    if (!m_readonly)
    {
        QToolButton *browseButton = new QToolButton();
        browseButton->setIcon(browseIcon);
        browseButton->setFixedHeight(24);
        connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
        m_hLayout->addWidget(browseButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    }
    //

    QObject::connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
                     this, SLOT(OnDoubleClick()));

    m_heightSizeHint = 35;

    SetValue(filepath);
    AfterConstructor();
}

AttrWidgetFile::~AttrWidgetFile()
{
}

void AttrWidgetFile::Browse()
{
    String selectedFile = "";
    DialogBrowseAssetFile *dialog = new DialogBrowseAssetFile(&selectedFile);
    dialog->Show(EditorWindow::GetInstance()->GetMainWindow(),
                 p_inspectorWidget->GetInspectableXMLInfo().GetTagName(),
                 m_fileExtension.Split(' ').ToList());
    while (dialog->isVisible()) { Application::processEvents(); }

    if (!selectedFile.Empty())
    {
        SetValue(selectedFile);
        p_inspectorWidget->_OnSlotValueChanged();
    }

    RefreshIcon();
}

void AttrWidgetFile::RefreshIcon()
{
    File file(m_filepath);
    File *f = File::GetSpecificFile(file);
    if (f)
    {
        const QPixmap& pm = f->GetIcon();
        QPixmap pmCopy = pm.scaled(16, 16, Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
        m_iconLabel->setPixmap(pmCopy);
        delete f;
    }
}

void AttrWidgetFile::SetValue(const String &filepath, bool draggedFile)
{
    ENSURE(m_filepath != filepath);

    m_filepath = filepath;

    // Change the filepath in the xmlAttribute too
    m_xmlAttribute.SetFilepath(m_filepath,
                               m_xmlAttribute.GetPropertyValue(
                                   XMLProperty::FileExtension.GetName()),
                               m_xmlAttribute.GetProperties());

    String fileName = IO::GetFileName(m_filepath);
    String fileText = !fileName.Empty() ? fileName : "None";

    m_filepathLineEdit->SetBold( !fileName.Empty() );
    blockSignals(true);
    m_filepathLineEdit->setText(fileText.ToQString());
    blockSignals(false);

    if (draggedFile)
    {
        p_inspectorWidget->_OnSlotValueChanged();
    }
    RefreshIcon();
}

String AttrWidgetFile::GetValue()
{
    return m_filepath;
}

void AttrWidgetFile::OnDragStart(const DragDropInfo &ddi)
{
    if (!m_readonly)
    {
        Explorer *explorer = Explorer::GetInstance();
        if (ddi.sourceObject == explorer)
        {
            File f = explorer->GetSelectedFile();
            String extensions =
                    m_xmlAttribute.GetPropertyValue(
                        XMLProperty::FileExtension.GetName());
            if (f.IsOfExtension(extensions))
            {
                m_filepathLineEdit->setStyleSheet(
                            IDragDropListener::acceptDragStyle);
            }
            else
            {
                m_filepathLineEdit->setStyleSheet(
                            IDragDropListener::rejectDragStyle);
            }
        }
    }
}

void AttrWidgetFile::OnDrop(const DragDropInfo &ddi)
{
    m_filepathLineEdit->setStyleSheet("/* */");

    Explorer  *explorer  = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (ddi.currentObject == this)
    {
        if (ddi.sourceObject == explorer)
        {
            File f = explorer->GetSelectedFile();
            if (f.IsOfExtension(m_fileExtension) && !m_readonly)
            {
                SetValue(f.GetAbsolutePath(), true);
            }
        }
        else if (ddi.sourceObject == hierarchy)
        {
            GameObject *go = hierarchy->GetFirstSelectedGameObject();
        }
    }
}


void AttrWidgetFile::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_filepath != "")
    {
        OnDoubleClick();
    }
}

void AttrWidgetFile::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::File) return;
    SetValue( attribute.GetFilepath() );
}

void AttrWidgetFile::OnDoubleClick()
{
    ENSURE(IO::ExistsFile(m_filepath));
    ENSURE(!IO::IsEngineFile(m_filepath));
    ENSURE( m_filepath.BeginsWith( IO::GetProjectRootAbs() ) );
    Explorer::GetInstance()->SelectFile(m_filepath);
}


////////////////////////////////////////////////////////

FileLineEdit::FileLineEdit(QWidget *parent) : QLineEdit(parent)
{
    setFixedHeight(24);
    setAlignment(Qt::AlignmentFlag::AlignLeft);
    connect (this, SIGNAL(selectionChanged()), this, SLOT(Deselect()));
}

void FileLineEdit::SetBold(bool bold)
{
    QFont currentFont = font();
    currentFont.setBold(bold);
    setFont( currentFont );
}

void FileLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit DoubleClicked();
}

void FileLineEdit::Deselect()
{
    deselect();
}
