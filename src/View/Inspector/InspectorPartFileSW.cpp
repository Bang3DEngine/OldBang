#include "InspectorPartFileSW.h"

InspectorPartFileSW::InspectorPartFileSW(const std::string &labelString,
                                           const std::string &assetPath,
                                           const std::string &fileExtension,
                                           InspectorPartWidget *parent) :
    InspectorPartSW(labelString, parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    this->setLayout(vLayout);

    QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
    textLabel->setContentsMargins(0,0,0,0);
    vLayout->addWidget(textLabel);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    filepathLineEdit = new QLineEdit();
    filepathLineEdit->setReadOnly(true);
    hLayout->addWidget(filepathLineEdit);

    QPushButton *browseButton = new QPushButton(QString::fromStdString("Browse"));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
    hLayout->addWidget(browseButton);

    this->fileExtension = fileExtension;
    this->setContentsMargins(0,0,0,0);
    this->show();
    this->SetValue(assetPath);
}

void InspectorPartFileSW::Browse()
{
    FileDialog fda(Explorer::GetTopPath(), fileExtension, this);

    std::string selectedFile = fda.GetOpenFileName();
    if(selectedFile != "")
    {
        this->SetValue( selectedFile );
        this->parent->_NotifyInspectorSlotChanged();
    }
}

void InspectorPartFileSW::SetValue(const std::string &assetPath)
{
    if(this->assetPath != assetPath)
    {
        this->assetPath = assetPath;
        filepathLineEdit->setText(QString::fromStdString(assetPath));
    }
}

std::string  InspectorPartFileSW::GetValue()
{
    return assetPath;
}
