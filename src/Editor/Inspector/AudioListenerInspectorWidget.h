#ifndef AUDIOLISTENERINSPECTORWIDGET_H
#define AUDIOLISTENERINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"

class AudioListener;
class AudioListenerInspectorWidget : public ComponentWidget
{
public:
    AudioListenerInspectorWidget(AudioListener *audioListener);
    virtual ~AudioListenerInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    AudioListener *p_audioListener = nullptr;
};

#endif // AUDIOLISTENERINSPECTORWIDGET_H
