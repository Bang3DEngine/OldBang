#ifndef AUDIOSOURCEINSPECTORWIDGET_H
#define AUDIOSOURCEINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"
#include "Bang/AttrWidgetButton.h"

class AudioSource;
class AudioSourceInspectorWidget : public ComponentWidget
{
public:
    AudioSourceInspectorWidget(AudioSource *audioSource);

protected:
    virtual void CreateWidgetSlots(const XMLNode &xmlInfo) override;

private:
    AttrWidgetButton *m_playStopButton = nullptr;
};

#endif // AUDIOSOURCEINSPECTORWIDGET_H
