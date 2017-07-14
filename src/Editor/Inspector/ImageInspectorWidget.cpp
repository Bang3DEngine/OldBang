#include "Bang/ImageInspectorWidget.h"

ImageInspectorWidget::ImageInspectorWidget(const ImageFile &ImageFile)
    : FileInspectorWidget(ImageFile)
{
}

ImageInspectorWidget::~ImageInspectorWidget()
{

}
void ImageInspectorWidget::OnDestroy()
{
    FileInspectorWidget::OnDestroy();
}

void ImageInspectorWidget::InitExtra()
{
    FileInspectorWidget::InitExtra();

    m_dimensionsAW = new AttrWidgetString("Dimensions");

    InsertAttributeWidget(m_dimensionsAW);
}

void ImageInspectorWidget::Refresh()
{
    FileInspectorWidget::Refresh();
    ImageFile imgFile(m_file.GetPath());
    m_dimensionsAW->SetValue(
        String::ToString(imgFile.GetWidth()) + " x " +
        String::ToString(imgFile.GetHeight()) );
}
