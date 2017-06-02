#ifndef FILEIMAGE_H
#define FILEIMAGE_H

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"
#include "Bang/IInspectable.h"

class ImageFile : public BFile
{
public:
    ImageFile();
    ImageFile(const Path& path);

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;

    void Read(const XMLNode &xmlInfo);
    void Write(XMLNode *xmlInfo) const;

protected:
    int m_width = 0, m_height = 0;
    int m_numComponents = 3;
};

#endif // FILEIMAGE_H
