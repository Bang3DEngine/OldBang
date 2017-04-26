#ifndef SHADERPROGRAMASSETFILE_H
#define SHADERPROGRAMASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

class ShaderProgramAssetFile : public File
{
public:
    ShaderProgramAssetFile();
    ShaderProgramAssetFile(const QFileSystemModel *model,
                           const QModelIndex &index);

    virtual const QPixmap& GetIcon() const override;

    String GetVertexShaderFilepath() const;
    String GetFragmentShaderFilepath() const;

    void Read(const XMLNode &xmlInfo) override;
    void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif
};

#endif // SHADERPROGRAMASSETFILE_H
