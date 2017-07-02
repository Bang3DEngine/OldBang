#ifndef FONTASSETFILE_H
#define FONTASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/IInspectable.h"

class FontFile : public BFile
{
public:
    FontFile();
    FontFile(const Path &path);

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
