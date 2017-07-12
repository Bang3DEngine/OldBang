#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"

class SerializableObject;
class Texture2DFile : public BFile
{
public:
    Texture2DFile();
    Texture2DFile(const Path& path);

    Path GetImageFilepath() const;
    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual bool IsAsset() const override;

private:
    XMLNode m_xmlInfo;
};

#endif // FILETEXTURE2DASSET_H
