#ifndef AUDIOSOURCEINSPECTORWIDGET_H
#define AUDIOSOURCEINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

class AudioSource;
class AudioSourceInspectorWidget : public ComponentWidget,
                                   public IAttrWidgetButtonListener
{
public:
    AudioSourceInspectorWidget(AudioSource *audioSource);
    virtual ~AudioSourceInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnButtonClicked(const AttrWidgetButton *clickedButton) override;
    void OnDestroy() override;

private:
    AudioSource *p_audioSource = nullptr;

    AttrWidgetFile   *m_audioClipFileAW  = nullptr;
    AttrWidgetFloat  *m_volumeAW         = nullptr;
    AttrWidgetFloat  *m_pitchAW          = nullptr;
    AttrWidgetFloat  *m_rangeAW          = nullptr;
    AttrWidgetBool   *m_loopingAW        = nullptr;
    AttrWidgetButton *m_playStopButtonAW = nullptr;
};

#endif // AUDIOSOURCEINSPECTORWIDGET_H
