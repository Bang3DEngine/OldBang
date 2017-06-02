#include "Bang/AttrWidgetFile.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "Bang/WinUndef.h"

#include "Bang/BFile.h"
#include "Bang/Paths.h"
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
    m_allowedExtensions = xmlAttribute.GetPropertyValue(
                                  XMLProperty::FileExtension.GetName());

    m_hLayout = new QHBoxLayout();
    m_layout.addLayout(m_hLayout, 1);

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
    QObject::connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
                     this, SLOT(OnDoubleClick()));
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


    m_heightSizeHint = 35;

    Path filepath;
    bool isEngineFile = xmlAttribute.HasProperty(XMLProperty::IsEngineFile);
    if (isEngineFile) { filepath = EPATH( xmlAttribute.GetValue() ); }
    else              { filepath = UPATH( xmlAttribute.GetValue() ); }
    SetValue(m_filepath);

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
                 m_allowedExtensions.Split(' ').To<List>());
    while (dialog->isVisible()) { Application::processEvents(); }

    Path selectedPath(selectedFile);
    if (!selectedPath.IsEmpty())
    {
        if (!selectedPath.IsFile()) { SetValue(Path::Empty); }
        else { SetValue(selectedPath); }
        p_inspectorWidget->_OnSlotValueChanged();
    }
}

void AttrWidgetFile::RefreshIcon()
{
    const uint c_pixSize = 16;

    BFile file(m_filepath);
    BFile *f = BFile::GetSpecificFile(file);
    if (file.GetPath().IsFile() && f)
    {
        const QPixmap& pm = f->GetIcon();
        QPixmap pmCopy = pm.scaled(c_pixSize, c_pixSize,
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
        m_iconLabel->setPixmap(pmCopy);
        delete f;
    }
    else
    {
        m_iconLabel->setPixmap( IconManager::GetEmptyPixmap()
                                             .scaled(c_pixSize, c_pixSize) );
    }
}

void AttrWidgetFile::SetValue(const Path &filepath, bool draggedFile)
{
    ENSURE(m_filepath != filepath);

    m_filepath = filepath;

    // Change the filepath in the xmlAttribute too
    m_xmlAttribute.SetFilepath(m_filepath,
                               m_xmlAttribute.GetPropertyValue(
                                   XMLProperty::FileExtension.GetName()),
                               m_xmlAttribute.GetProperties());

    String fileName = m_filepath.GetName();
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
    return m_filepath.GetName();
}

void AttrWidgetFile::OnDragStart(const DragDropInfo &ddi)
{
    if (!m_readonly)
    {
        Explorer *explorer = Explorer::GetInstance();
        if (ddi.sourceObject == explorer)
        {
            BFile f = explorer->GetSelectedFile();
            String extensions =
                    m_xmlAttribute.GetPropertyValue(
                        XMLProperty::FileExtension.GetName());
            if (f.GetPath().HasExtension(extensions))
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
    if (ddi.currentObject == this)
    {
        if (ddi.sourceObject == explorer)
        {
            BFile f = explorer->GetSelectedFile();
            if (f.GetPath().HasExtension( m_allowedExtensions ) &&
                !m_readonly)
            {
                SetValue(f.GetPath(), true);
            }
        }
    }
}


void AttrWidgetFile::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!m_filepath.IsEmpty()) { OnDoubleClick(); }
}

void AttrWidgetFile::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::File) return;
    SetValue( attribute.GetFilepath() );
}

const Path &AttrWidgetFile::GetPath() const
{
    return m_filepath;
}

void AttrWidgetFile::OnDoubleClick()
{
    ENSURE(m_filepath.Exists());
    ENSURE(m_filepath.BeginsWith( Paths::Project() ));
    Explorer::GetInstance()->SelectPath(m_filepath);
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
