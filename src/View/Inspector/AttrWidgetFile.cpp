#include "AttrWidgetFile.h"

AttrWidgetFile::AttrWidgetFile(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    m_fileExtension = xmlAttribute.GetPropertyValue(
                        XMLProperty::FileExtension.GetName());

    QHBoxLayout *hLayout = new QHBoxLayout();
    m_layout->addLayout(hLayout, 1);

    m_filepathLineEdit = new FileLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    m_filepathLineEdit->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(m_filepathLineEdit, 100);

    if (!m_readonly)
    {
        hLayout->addStretch(1); // add little spacer
        QPushButton *browseButton = new QPushButton(QString("Browse"));
        browseButton->setFixedHeight(24);
        connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
        hLayout->addWidget(browseButton, 20, Qt::AlignRight | Qt::AlignVCenter);
    }

    connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
            this, SLOT(OnDoubleClick()));

    AfterConstructor();
}

void AttrWidgetFile::Browse()
{
    FileDialog fd(Persistence::GetAssetsPathAbsolute(), m_fileExtension);
    std::string selectedFile = fd.GetOpenFilename();
    if (selectedFile != "")
    {
        SetValue(selectedFile);
        m_inspectorWidget->_OnSlotValueChanged();
    }
}

void AttrWidgetFile::SetValue(const std::string &filepath, bool draggedFile)
{
    if (m_filepath != filepath)
    {
        m_filepath = filepath;

        std::string value = Persistence::GetFileName(m_filepath);
        m_filepathLineEdit->setText(QString::fromStdString(value));
    }

    if (draggedFile)
    {
        m_inspectorWidget->_OnSlotValueChanged();
    }
}

std::string  AttrWidgetFile::GetValue()
{
    return m_filepath;
}

void AttrWidgetFile::OnDropFromExplorer(const File &f, QDropEvent *e)
{
    if (f.IsOfExtension(m_fileExtension) && !m_readonly)
    {
        SetValue(f.GetRelativePath(), true);
    }
}

void AttrWidgetFile::OnDropFromHierarchy(GameObject *go, QDropEvent *e)
{
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
