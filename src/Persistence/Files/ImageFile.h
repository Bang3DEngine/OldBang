#ifndef FILEIMAGE_H
#define FILEIMAGE_H

#include "Bang/File.h"

class ImageFile : public File
{
public:
    ImageFile();
    ImageFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;

protected:
    int m_width = 0, m_height = 0;
    int m_numComponents = 3;
};

#endif // FILEIMAGE_H
