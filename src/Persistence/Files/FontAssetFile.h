#ifndef FONTASSETFILE_H
#define FONTASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/String.h"

class FontAssetFile : public File
{
public:
    FontAssetFile();
    FontAssetFile(const Path &path);

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

    const Path &GetFontFilepath() const;

private:
    Path m_trueFontFilepath;
};

#endif // FONTASSETFILE_H
