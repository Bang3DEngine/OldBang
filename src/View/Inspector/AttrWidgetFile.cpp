#include "AttrWidgetFile.h"

AttrWidgetFile::AttrWidgetFile(const std::string &labelString,
                               const std::string &fileExtension,
                               InspectorWidget *parent) :
    AttributeWidget(labelString, parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    setLayout(vLayout);

    vLayout->addWidget(GetLabelWidget(labelString));

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    m_filepathLineEdit = new FileLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    hLayout->addWidget(m_filepathLineEdit);

    QPushButton *browseButton = new QPushButton(QString("Browse"));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
    hLayout->addWidget(browseButton);

    connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
            this, SLOT(OnDoubleClick()));

    m_fileExtension = fileExtension;
    setContentsMargins(0,0,0,0);
    show();
}

void AttrWidgetFile::Browse()
{
    FileDialog fd(Persistence::GetAssetsPathAbsolute(), m_fileExtension);
    std::string selectedFile = fd.GetOpenFilename();
    if (selectedFile != "")
    {
        SetValue(selectedFile);
        m_parent->_OnSlotValueChanged();
    }
}

void AttrWidgetFile::SetValue(const std::string &filepath)
{
    if (m_filepath != filepath)
    {
        m_filepath = filepath;
        m_filepathLineEdit->setText(QString::fromStdString(filepath));
    }
}

std::string  AttrWidgetFile::GetValue()
{
    return m_filepath;
}

void AttrWidgetFile::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_filepath != "")
    {
        OnDoubleClick();
    }
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
}

void FileLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit DoubleClicked();
}

void FileLineEdit::Deselect()
{
    deselect();
}
