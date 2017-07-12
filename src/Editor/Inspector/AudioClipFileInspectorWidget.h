#ifndef AUDIOCLIPFILEINSPECTORWIDGET_H
#define AUDIOCLIPFILEINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

class AudioClipFile;
class AttrWidgetButton;
class AudioClipFileInspectorWidget : public InspectorWidget,
                                     public IAttrWidgetButtonListener
{
public:
    AudioClipFileInspectorWidget(const AudioClipFile &audioClipFile);
    virtual ~AudioClipFileInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
    void OnButtonClicked(const AttrWidgetButton *clickedButton) override;
};


#endif // AUDIOCLIPFILEINSPECTORWIDGET_H
