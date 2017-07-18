#ifndef MATERIALASSETFILE_H
#define MATERIALASSETFILE_H

#include <QPixmap>

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector4.h"
#include "Bang/XMLNode.h"

class MaterialFile : public BFile
{
public:
    MaterialFile();
    MaterialFile(const Path& path);

    void Read(const XMLNode &xmlInfo);
    void Write(XMLNode *xmlInfo) const;

    virtual bool IsAsset() const override;

private:
    mutable XMLNode m_xmlInfo;
    Path m_vshaderFilepath;
    Path m_fshaderFilepath;
    Array<Path> m_textureFilepaths;
    Color m_diffuseColor = Color(0.0f);
};

#endif // MATERIALASSETFILE_H
