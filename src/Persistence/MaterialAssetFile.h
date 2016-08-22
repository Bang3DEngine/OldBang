#ifndef MATERIALASSETFILE_H
#define MATERIALASSETFILE_H

#include "Bang.h"

#include "File.h"
#include "XMLParser.h"


#include <vector>

class MaterialAssetFile : public File
{
private:
    String m_vshaderFilepath = "";
    String m_fshaderFilepath = "";
    int m_textureCount = 0;
    std::vector< String > m_textureFilepaths;
    Vector4 m_diffuseColor = Vector4(0.0f);

public:
    MaterialAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual QPixmap GetIcon() const override;
    virtual IInspectable *GetInspectable() const override;

    const String& GetVertexShaderFilepath() const;
    const String& GetFragmentShaderFilepath() const;
    int GetTextureCount() const;
    const std::vector<String>& GetTextureFilepaths() const;
    const Vector4& GetDiffuseColor() const;
};

#endif // MATERIALASSETFILE_H
