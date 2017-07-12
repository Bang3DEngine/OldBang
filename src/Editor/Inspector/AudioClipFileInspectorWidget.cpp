#include "Bang/AudioClipFileInspectorWidget.h"

#include "Bang/AudioClipFile.h"

AudioClipFileInspectorWidget::AudioClipFileInspectorWidget(
        const AudioClipFile &audioClipFile)
    : InspectorWidget()
{

}

AudioClipFileInspectorWidget::~AudioClipFileInspectorWidget()
{

}
void AudioClipFileInspectorWidget::OnDestroy()
{
    InspectorWidget::OnDestroy();
}

void AudioClipFileInspectorWidget::OnButtonClicked(const AttrWidgetButton *clickedButton)
{

}


void AudioClipFileInspectorWidget::CreateAttributeWidgets(const XMLNode &xmlInfo)
{
    InspectorWidget::CreateAttributeWidgets(xmlInfo);
}

void AudioClipFileInspectorWidget::Refresh()
{
    InspectorWidget::Refresh();
}
