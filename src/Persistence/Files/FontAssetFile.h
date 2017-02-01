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
