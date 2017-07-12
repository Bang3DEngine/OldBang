#ifndef AUDIOSOURCEINSPECTORWIDGET_H
#define AUDIOSOURCEINSPECTORWIDGET_H

#include <QPushButton>
#include "Bang/WinUndef.h"

#include "Bang/ComponentWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

class AudioSource;
class AttrWidgetButton;
class AudioSourceInspectorWidget : public ComponentWidget,
                                   public IAttrWidgetButtonListener
{
public:
    AudioSourceInspectorWidget(AudioSource *audioSource);
    ~AudioSourceInspectorWidget();

protected:
    void CreateAttributeWidgets(const XMLNode &xmlInfo) override;
    void Refresh() override;
    void OnButtonClicked(const AttrWidgetButton *clickedButton) override;
    void OnDestroy() override;

private:
    AudioSource *p_audioSource = nullptr;
    AttrWidgetButton *m_playStopButton = nullptr;
};

#endif // AUDIOSOURCEINSPECTORWIDGET_H
