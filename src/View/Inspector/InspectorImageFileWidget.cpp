#include "InspectorImageFileWidget.h"

InspectorImageFileWidget::InspectorImageFileWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    xmlInfo->SetString("FileName", fileImage.GetName() + "." + fileImage.GetExtension(),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Path", fileImage.GetPath(), {XMLProperty::Readonly});
    xmlInfo->SetString("Dimensions", std::to_string(fileImage.GetWidth()) + "x" +
                                     std::to_string(fileImage.GetHeight()),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA",
                       {XMLProperty::Readonly});

    //ConstructFromWidgetXMLInfo(fileImage.GetName(), *xmlInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
