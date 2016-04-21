#include "InspectorFileSW.h"

InspectorFileSW::InspectorFileSW(const std::string &labelString,
                                           const std::string &assetPath,
                                           const std::string &fileExtension,
                                           InspectorWidget *parent) :
    InspectorSW(labelString, parent)
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

void InspectorFileSW::Browse()
{
    FileDialog fda(Persistence::GetAssetsPathAbsolute(), fileExtension, this);

    std::string selectedFile = fda.GetOpenFileName();
    if(selectedFile != "")
    {
        this->SetValue( selectedFile );
        this->parent->_OnSlotValueChanged();
    }
}

void InspectorFileSW::SetValue(const std::string &assetPath)
{
    if(this->assetPath != assetPath)
    {
        this->assetPath = assetPath;
        filepathLineEdit->setText(QString::fromStdString(assetPath));
    }
}

std::string  InspectorFileSW::GetValue()
{
    return assetPath;
}
