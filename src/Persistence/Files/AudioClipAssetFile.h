#ifndef AUDIOCLIPASSETFILE_H
#define AUDIOCLIPASSETFILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

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
    AudioClipAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual ~AudioClipAssetFile();

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetAudioFilepath(const String &audioFilepath);
    const String& GetAudioFilepath() const;

    virtual void OnButtonClicked(const String &attrName);

    virtual bool IsAsset() const override;

private:
    String m_audioFilepath = "";
    AudioSource *m_tmpAudioSource = nullptr;

    AudioClip *GetRelatedAudioClip() const;
};

#endif // AUDIOCLIPASSETFILE_H
