#include "Bang/FileInspectorWidget.h"

#include "Bang/Debug.h"

FileInspectorWidget::FileInspectorWidget(const File &file)
    : InspectorWidget()
{
    m_file = file;
}

FileInspectorWidget::~FileInspectorWidget()
{

}
void FileInspectorWidget::OnDestroy()
{
    InspectorWidget::OnDestroy();
}

void FileInspectorWidget::InitExtra()
{
    InspectorWidget::InitExtra();

    SetTitle(m_file.GetPath().GetName());
}

void FileInspectorWidget::Refresh()
{
    InspectorWidget::Refresh();
}
