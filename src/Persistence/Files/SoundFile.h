#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

#ifdef BANG_EDITOR
#include "Bang/IAttrWidgetButtonListener.h"
#endif

class AudioClip;
class AudioSource;
class SoundFile : public File
                  #ifdef BANG_EDITOR
                  ,public IAttrWidgetButtonListener
                  #endif
{
public:
    SoundFile();
    SoundFile(const Path& path);
    virtual ~SoundFile();

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    virtual void OnButtonClicked(const String &attrName) override;
    #endif

private:
    AudioSource* m_tmpAudioSource = nullptr;
    AudioClip* m_tmpAudioClip     = nullptr;

};

#endif // AUDIOFILE_H
