#include "Bang/AudioSourceInspectorWidget.h"

#include "Bang/Debug.h"
#include "Bang/EditorState.h"
#include "Bang/AudioSource.h"
#include "Bang/AttrWidgetButton.h"

AudioSourceInspectorWidget::AudioSourceInspectorWidget(AudioSource *audioSource)
    : ComponentWidget(audioSource)
{
    p_audioSource = audioSource;
}

AudioSourceInspectorWidget::~AudioSourceInspectorWidget()
{
}
void AudioSourceInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
    if (EditorState::IsStopped())
    {
        p_audioSource->Stop();
    }
}


void AudioSourceInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_playStopButton = new AttrWidgetButton("Play", this);
    m_playStopButton->SetValue("Play");
    InsertAttributeWidget(m_playStopButton);
}

void AudioSourceInspectorWidget::Refresh()
{
    InspectorWidget::Refresh();
    m_playStopButton->SetValue( p_audioSource->IsPlaying() ? "Stop" : "Play" );
}

void AudioSourceInspectorWidget::OnButtonClicked(const AttrWidgetButton *clickedButton)
{
    if (clickedButton == m_playStopButton)
    {
        if (p_audioSource->IsPlaying()) { p_audioSource->Stop(); }
        else { p_audioSource->Play(); }
    }
}
