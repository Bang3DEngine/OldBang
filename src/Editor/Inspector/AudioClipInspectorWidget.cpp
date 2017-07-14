#include "Bang/AudioClipInspectorWidget.h"

AudioClipInspectorWidget::AudioClipInspectorWidget(
        const AudioClipFile &audioClipFile)
    : AssetInspectorWidget(audioClipFile)
{
}

AudioClipInspectorWidget::~AudioClipInspectorWidget()
{

}
void AudioClipInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void AudioClipInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();
    m_soundFileAW = new AttrWidgetFile("Sound File");
    m_lengthAW = new AttrWidgetString("Length");

    InsertAttributeWidget(m_soundFileAW);
    InsertAttributeWidget(m_lengthAW);
}

void AudioClipInspectorWidget::Refresh()
{
    AssetInspectorWidget::Refresh();

    AudioClip *ac = GetAsset();
    if (ac)
    {
        m_soundFileAW->SetValue( ac->GetSoundFilepath() );
        m_lengthAW->SetValue( String::ToString(ac->GetLength(), 2) + " seconds");
    }
}

void AudioClipInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    AudioClip *ac = GetAsset();
    if (attrWidget == m_soundFileAW)
    {
        ac->LoadFromFile(m_soundFileAW->GetPath());
    }

    RewriteAsset();
}
