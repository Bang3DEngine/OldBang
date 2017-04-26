#ifndef MATERIALASSETFILE_H
#define MATERIALASSETFILE_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector4.h"
#include "Bang/XMLNode.h"

class MaterialAssetFile : public File
{
public:
    MaterialAssetFile();
    MaterialAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    void Read(const XMLNode &xmlInfo);
    void Write(XMLNode *xmlInfo) const;

    virtual bool IsAsset() const override;

    const String& GetVertexShaderFilepath() const;
    const String& GetFragmentShaderFilepath() const;
    int GetTextureCount() const;
    const Array<String>& GetTextureFilepaths() const;
    const Color& GetDiffuseColor() const;

private:
    mutable XMLNode m_xmlInfo;
    String m_vshaderFilepath = "";
    String m_fshaderFilepath = "";
    Array< String > m_textureFilepaths;
    Color m_diffuseColor = Color(0.0f);
};

#endif // MATERIALASSETFILE_H
