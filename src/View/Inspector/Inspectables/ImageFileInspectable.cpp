#include "Bang/ImageFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"
#include "Bang/IconManager.h"
#include "Bang/InspectorWidget.h"

ImageFileInspectable::ImageFileInspectable(const ImageFile &fileImage) :
    m_fileImage(fileImage)
{
}

const QPixmap &ImageFileInspectable::GetIcon() const
{
    String path = IO::ToAbsolute("Icons/ImageIcon.png", true);
    return IconManager::LoadPixmap(path);
}

void ImageFileInspectable::Read(const XMLNode &xmlInfo)
{
}

void ImageFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("ImageFileInspectable");

    xmlInfo->SetString("FileName", m_fileImage.GetName() +
                                  "." + m_fileImage.GetExtension(),
                       {XMLProperty::Readonly});
    xmlInfo->SetFilepath("ImageFilePath", m_fileImage.GetRelativePath(), "",
                         {XMLProperty::Readonly});
    xmlInfo->SetString("Dimensions", std::to_string(m_fileImage.GetWidth()) + "x" +
                                     std::to_string(m_fileImage.GetHeight()),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", m_fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA",
                       {XMLProperty::Readonly});
}

