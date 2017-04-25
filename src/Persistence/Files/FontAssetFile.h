#ifndef FONTASSETFILE_H
#define FONTASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/String.h"

class SerializableObject;
class FontAssetFile : public File
{
public:
    FontAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    const String &GetFontFilepath() const;

private:
    String m_trueFontFilepath = "";
};

#endif // FONTASSETFILE_H
