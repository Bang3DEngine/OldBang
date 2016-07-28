#include "InspectorImageFileWidget.h"

InspectorImageFileWidget::
InspectorImageFileWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    xmlInfo->SetAttribute("FileName", fileImage.GetName() + "." + fileImage.GetExtension());
    xmlInfo->SetAttribute("Path", fileImage.GetPath());
    xmlInfo->SetAttribute("Dimensions", std::to_string(fileImage.GetWidth()) + "x" +
                                     std::to_string(fileImage.GetHeight()));
    xmlInfo->SetAttribute("Mode", fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA");

    ConstructFromWidgetXMLInfo(fileImage.GetName(), *xmlInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
