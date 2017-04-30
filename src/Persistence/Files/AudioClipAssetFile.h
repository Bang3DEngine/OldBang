#ifndef AUDIOCLIPASSETFILE_H
#define AUDIOCLIPASSETFILE_H

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/IAttrWidgetButtonListener.h"

class AudioClip;
class AudioSource;
class AudioClipAssetFile : public File,
                           public IAttrWidgetButtonListener
{
public:
    AudioClipAssetFile();
    AudioClipAssetFile(const Path& path);

    virtual ~AudioClipAssetFile();

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetSoundFilepath(const Path &audioFilepath);
    const Path& GetAudioFilepath() const;

    virtual void OnButtonClicked(const String &attrName);

    virtual bool IsAsset() const override;

private:
    Path m_audioFilepath;
    AudioSource *m_tmpAudioSource = nullptr;

    AudioClip *GetRelatedAudioClip() const;
};

#endif // AUDIOCLIPASSETFILE_H
