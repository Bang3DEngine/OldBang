#ifndef FILEIMAGE_H
#define FILEIMAGE_H

#include "stb_image.h"
#include "Logger.h"

#include "File.h"

class ImageFile : public File
{
protected:
    int m_width = 0, m_height = 0;
    int m_numComponents = 3;

public:
    ImageFile(const QFileSystemModel *model, const QModelIndex *index);

    virtual QPixmap GetIcon() const override;
    virtual IInspectable *GetInspectable() const override;

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;
};

#endif // FILEIMAGE_H
