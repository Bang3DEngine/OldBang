#ifndef SOUNDINSPECTORWIDGET_H
#define SOUNDINSPECTORWIDGET_H

#include "Bang/SoundFile.h"
#include "Bang/FileInspectorWidget.h"

class SoundInspectorWidget : public FileInspectorWidget
{
public:
    SoundInspectorWidget(const SoundFile &soundFile);
    virtual ~SoundInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;
};

#endif // SOUNDINSPECTORWIDGET_H
