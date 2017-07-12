#include "Bang/AudioListenerInspectorWidget.h"

#include "Bang/AudioListener.h"

AudioListenerInspectorWidget::AudioListenerInspectorWidget(
        AudioListener *audioListener)
    : ComponentWidget(audioListener)
{
    p_audioListener = audioListener;
}

AudioListenerInspectorWidget::~AudioListenerInspectorWidget()
{

}

void AudioListenerInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();
}

void AudioListenerInspectorWidget::Refresh()
{
    ComponentWidget::Refresh();
}

void AudioListenerInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{

}

void AudioListenerInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
