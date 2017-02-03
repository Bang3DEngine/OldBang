#ifndef FONTASSETFILE_H
#define FONTASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "File.h"
#include "String.h"

class IInspectable;
class FontAssetFile : public File
{
public:
    FontAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    const String &GetFontFilepath() const;

private:
    String m_trueFontFilepath = "";
};

#endif // FONTASSETFILE_H
