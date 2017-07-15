#ifndef AUDIOCLIPINSPECTORWIDGET_H
#define AUDIOCLIPINSPECTORWIDGET_H

#include "Bang/AudioClip.h"
#include "Bang/AudioClipFile.h"
#include "Bang/AssetInspectorWidget.h"

class AudioClipInspectorWidget : public AssetInspectorWidget<AudioClip>
{
public:
    AudioClipInspectorWidget(const AudioClipFile &audioClipFile);
    virtual ~AudioClipInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;

    AttrWidgetFile   *m_soundFileAW = nullptr;
    AttrWidgetString *m_lengthAW = nullptr;
};


#endif // AUDIOCLIPINSPECTORWIDGET_H
