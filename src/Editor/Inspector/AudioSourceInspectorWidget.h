#ifndef AUDIOSOURCEINSPECTORWIDGET_H
#define AUDIOSOURCEINSPECTORWIDGET_H

#include <QPushButton>
#include "Bang/WinUndef.h"

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
    QPushButton *m_playStopButton = nullptr;
};

#endif // AUDIOSOURCEINSPECTORWIDGET_H
