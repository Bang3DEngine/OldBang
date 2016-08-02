#include "ImageFileInspectable.h"

ImageFileInspectable::ImageFileInspectable(const ImageFile &fileImage) :
    m_fileImage(fileImage)
{
}

void ImageFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
}

void ImageFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetString("FileName", m_fileImage.GetName() + "." + m_fileImage.GetExtension(),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Path", m_fileImage.GetPath(),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Dimensions", std::to_string(m_fileImage.GetWidth()) + "x" +
                                     std::to_string(m_fileImage.GetHeight()),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", m_fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA",
                       {XMLProperty::Readonly});
}
