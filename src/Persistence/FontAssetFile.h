#ifndef FONTASSETFILE_H
#define FONTASSETFILE_H

#include "Debug.h"

#include "File.h"


class FontAssetFile : public File
{
private:
    String m_trueFontFilepath = "";

public:
    FontAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif

    const String &GetFontFilepath() const;
};

#endif // FONTASSETFILE_H
