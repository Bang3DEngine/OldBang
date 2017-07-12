#include "Bang/AudioSourceInspectorWidget.h"

#include "Bang/Debug.h"
#include "Bang/AudioClip.h"
#include "Bang/EditorState.h"
#include "Bang/AudioSource.h"
#include "Bang/AssetsManager.h"
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

    m_audioClipFileAW  = new AttrWidgetFile("Audio Clip");
    m_volumeAW         = new AttrWidgetFloat("Volume");
    m_pitchAW          = new AttrWidgetFloat("Pitch");
    m_rangeAW          = new AttrWidgetFloat("Range");
    m_loopingAW        = new AttrWidgetBool("Looping");
    m_playStopButtonAW = new AttrWidgetButton("Play", this);

    InsertAttributeWidget(m_audioClipFileAW);
    InsertAttributeWidget(m_volumeAW);
    InsertAttributeWidget(m_pitchAW);
    InsertAttributeWidget(m_rangeAW);
    InsertAttributeWidget(m_loopingAW);
    InsertAttributeWidget(m_playStopButtonAW);
}

void AudioSourceInspectorWidget::Refresh()
{
    InspectorWidget::Refresh();
    m_audioClipFileAW->SetValue( p_audioSource->GetAudioClip()->GetFilepath() );
    m_volumeAW->SetValue(p_audioSource->GetVolume());
    m_pitchAW->SetValue(p_audioSource->GetPitch());
    m_rangeAW->SetValue(p_audioSource->GetRange());
    m_loopingAW->SetValue(p_audioSource->IsLooping());
    m_playStopButtonAW->SetValue( p_audioSource->IsPlaying() ? "Stop" : "Play" );
}

void AudioSourceInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    if (attrWidget == m_audioClipFileAW)
    {
        Path acPath = m_audioClipFileAW->GetPath();
        AudioClip *ac = AssetsManager::Load<AudioClip>(acPath);
        p_audioSource->SetAudioClip(ac);
    }
    else if (attrWidget == m_volumeAW)
    {
        p_audioSource->SetVolume(m_volumeAW->GetValue());
    }
    else if (attrWidget == m_pitchAW)
    {
        p_audioSource->SetPitch(m_pitchAW->GetValue());
    }
    else if (attrWidget == m_rangeAW)
    {
        p_audioSource->SetRange(m_rangeAW->GetValue());
    }
    else if (attrWidget == m_loopingAW)
    {
        p_audioSource->SetLooping(m_loopingAW->GetValue());
    }
}

void AudioSourceInspectorWidget::OnButtonClicked(const AttrWidgetButton *clickedButton)
{
    if (clickedButton == m_playStopButtonAW)
    {
        if (p_audioSource->IsPlaying()) { p_audioSource->Stop(); }
        else { p_audioSource->Play(); }
    }
}
