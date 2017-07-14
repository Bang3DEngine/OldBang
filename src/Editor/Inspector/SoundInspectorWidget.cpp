#include "Bang/SoundInspectorWidget.h"

SoundInspectorWidget::SoundInspectorWidget(const SoundFile &soundFile)
    : FileInspectorWidget(soundFile)
{
}

SoundInspectorWidget::~SoundInspectorWidget()
{

}
void SoundInspectorWidget::OnDestroy()
{
    FileInspectorWidget::OnDestroy();
}

void SoundInspectorWidget::InitExtra()
{
    FileInspectorWidget::InitExtra();
}

void SoundInspectorWidget::Refresh()
{
    FileInspectorWidget::Refresh();
}
