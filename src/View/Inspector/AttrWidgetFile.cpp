#include "AttrWidgetFile.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "File.h"
#include "Dialog.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "Persistence.h"
#include "InspectorWidget.h"

AttrWidgetFile::AttrWidgetFile(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    m_fileExtension = xmlAttribute.GetPropertyValue(
                        XMLProperty::FileExtension.GetName());

    QHBoxLayout *hLayout = new QHBoxLayout();
    m_layout->addLayout(hLayout, 1);

    // Icon
    String filepath = xmlAttribute.GetValue();
    filepath = Persistence::ToAbsolute(filepath, xmlAttribute.HasProperty(XMLProperty::IsEngineFile));
    File file(filepath);
    File *f = File::GetSpecificFile(file);
    if (f)
    {
        QPixmap pm = f->GetIcon();
        pm = pm.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QLabel *icon = new QLabel();
        icon->setPixmap(pm);
        hLayout->addWidget(icon, 0, Qt::AlignRight);
        delete f;
    }
    //

    // File Line Edit
    m_filepathLineEdit = new FileLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    m_filepathLineEdit->setAlignment(Qt::AlignLeft);
    m_filepathLineEdit->setMinimumWidth(50);
    hLayout->addWidget(m_filepathLineEdit, 100);
    //

    // Browse button
    if (!m_readonly)
    {
        hLayout->addStretch(1); // add little spacer
        QPushButton *browseButton = new QPushButton(QString("Browse"));
        browseButton->setFixedHeight(24);
        connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
        hLayout->addWidget(browseButton, 20, Qt::AlignRight | Qt::AlignVCenter);
    }
    //

    connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
            this, SLOT(OnDoubleClick()));

    AfterConstructor();
}

AttrWidgetFile::~AttrWidgetFile()
{
}

void AttrWidgetFile::Browse()
{
    String selectedFile =
            Dialog::GetOpenFilename(Persistence::GetAssetsPathAbsolute(),
                                        m_fileExtension);
    if (selectedFile != "")
    {
        SetValue(selectedFile);
        m_inspectorWidget->_OnSlotValueChanged();
    }
}

void AttrWidgetFile::SetValue(const String &filepath, bool draggedFile)
{
    ASSERT(m_filepath != filepath);

    m_filepath = filepath;

    // Change the filepath in the xmlAttribute too
    m_xmlAttribute.SetFilepath(m_filepath,
                               m_xmlAttribute.GetPropertyValue(
                                   XMLProperty::FileExtension.GetName()),
                               m_xmlAttribute.GetProperties());

    String fileName = Persistence::GetFileName(m_filepath);
    m_filepathLineEdit->setText(fileName.ToQString());

    if (draggedFile)
    {
        m_inspectorWidget->_OnSlotValueChanged();
    }
}

String  AttrWidgetFile::GetValue()
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
                    m_xmlAttribute.GetPropertyValue(XMLProperty::FileExtension.GetName());
            if (f.IsOfExtension(extensions))
            {
                m_filepathLineEdit->setStyleSheet(IDragDropListener::acceptDragStyle);
            }
            else
            {
                m_filepathLineEdit->setStyleSheet(IDragDropListener::rejectDragStyle);
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
    ASSERT(Persistence::ExistsFile(m_filepath));
    ASSERT(!Persistence::IsEngineFile(m_filepath));
    Explorer::GetInstance()->SelectFile(m_filepath);
}


////////////////////////////////////////////////////////

FileLineEdit::FileLineEdit(QWidget *parent) : QLineEdit(parent)
{
    connect (this, SIGNAL(selectionChanged()),
             this, SLOT(Deselect()));

    setFixedHeight(24);
    setAlignment(Qt::AlignmentFlag::AlignLeft);
}

void FileLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit DoubleClicked();
}

void FileLineEdit::Deselect()
{
    deselect();
}
