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

const File &FileInspectorWidget::GetFile() const
{
    return m_file;
}

void FileInspectorWidget::InitExtra()
{
    InspectorWidget::InitExtra();

    SetTitle(m_file.GetPath().GetName());
}

void FileInspectorWidget::OnUpdate()
{
    InspectorWidget::OnUpdate();
}

void FileInspectorWidget::OnDestroy()
{
    InspectorWidget::OnDestroy();
}
