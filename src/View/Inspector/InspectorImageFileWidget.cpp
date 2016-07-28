#include "InspectorImageFileWidget.h"

InspectorImageFileWidget::
InspectorImageFileWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    xmlInfo->SetString("FileName", fileImage.GetName() + "." + fileImage.GetExtension());
    xmlInfo->SetString("Path", fileImage.GetPath());
    xmlInfo->SetString("Dimensions", std::to_string(fileImage.GetWidth()) + "x" +
                                     std::to_string(fileImage.GetHeight()));
    xmlInfo->SetString("Mode", fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA");

    ConstructFromWidgetXMLInfo(fileImage.GetName(), *xmlInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
