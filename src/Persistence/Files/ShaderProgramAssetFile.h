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
    ShaderProgramAssetFile(const QFileSystemModel *model,
                           const QModelIndex &index);

    virtual const QPixmap& GetIcon() const override;

    String GetVertexShaderFilepath() const;
    String GetFragmentShaderFilepath() const;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() const override;
    #endif
};

#endif // SHADERPROGRAMASSETFILE_H
