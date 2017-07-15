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

void AudioListenerInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();
}

void AudioListenerInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);
    emit Changed(this);
}

void AudioListenerInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
