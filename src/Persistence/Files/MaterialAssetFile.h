#ifndef MATERIALASSETFILE_H
#define MATERIALASSETFILE_H

#include <QPixmap>

#include "File.h"
#include "Array.h"
#include "Color.h"
#include "String.h"
#include "Vector4.h"

class MaterialAssetFile : public File
{
private:
    String m_vshaderFilepath = "";
    String m_fshaderFilepath = "";
    Array< String > m_textureFilepaths;
    Color m_diffuseColor = Color(0.0f);

public:
    MaterialAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual SerializableObject *GetInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    const String& GetVertexShaderFilepath() const;
    const String& GetFragmentShaderFilepath() const;
    int GetTextureCount() const;
    const Array<String>& GetTextureFilepaths() const;
    const Color& GetDiffuseColor() const;
};

#endif // MATERIALASSETFILE_H
