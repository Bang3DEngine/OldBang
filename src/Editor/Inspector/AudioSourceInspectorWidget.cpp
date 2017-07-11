#include "Bang/AudioSourceInspectorWidget.h"

#include "Bang/Debug.h"
#include "Bang/AudioSource.h"

AudioSourceInspectorWidget::AudioSourceInspectorWidget(AudioSource *audioSource)
    : ComponentWidget(audioSource)
{
    /*
    AudioSource *noConstThis = const_cast<AudioSource*>(this);
    if (IsPlaying())
    {
        xmlInfo->SetButton("Stop", noConstThis, {});
        xmlInfo->SetButton("Play", noConstThis, {XMLProperty::Hidden});
    }
    else
    {

        xmlInfo->SetButton("Stop", noConstThis, {XMLProperty::Hidden});
        xmlInfo->SetButton("Play", noConstThis, {});
    }
    */
}

void AudioSourceInspectorWidget::CreateWidgetSlots(const XMLNode &xmlInfo)
{
    ComponentWidget::CreateWidgetSlots(xmlInfo);

    /*
    m_playStopButton = new QPushButton("Play");
    m_playStopButton->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    connect(m_playStopButton, SIGNAL(pressed()),
            this, SLOT(OnPlayStopButtonClicked()));

    QGridLayout *gridLayout = GetGridLayout();
    gridLayout->addWidget(m_playStopButton, GetNextRowIndex(), 0, 1,
                          colSpan, Qt::AlignLeft | Qt::AlignVCenter)

    AfterConstructor();
    m_layout.setAlignment(Qt::AlignRight);
    InspectorWidget.GetGridLayout()
    */
}
