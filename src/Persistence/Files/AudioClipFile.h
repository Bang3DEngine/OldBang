#ifndef AUDIOCLIPASSETFILE_H
#define AUDIOCLIPASSETFILE_H

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"

#ifdef BANG_EDITOR
#include "Bang/IAttrWidgetButtonListener.h"
#endif

class AudioClip;
class AudioSource;
class AudioClipFile : public File
                      #ifdef BANG_EDITOR
                      ,public IAttrWidgetButtonListener
                      #endif
{
public:
    AudioClipFile();
    AudioClipFile(const Path& path);

    virtual ~AudioClipFile();

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    virtual void OnButtonClicked(const String &attrName);
    #endif

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetSoundFilepath(const Path &audioFilepath);
    const Path& GetAudioFilepath() const;


    virtual bool IsAsset() const override;

private:
    Path m_audioFilepath;
    AudioSource *m_tmpAudioSource = nullptr;

    AudioClip *GetRelatedAudioClip() const;
};

#endif // AUDIOCLIPASSETFILE_H
