#ifndef SHADERPROGRAMASSETFILE_H
#define SHADERPROGRAMASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

class ShaderProgramFile : public File
{
public:
    ShaderProgramFile();
    ShaderProgramFile(const Path& path);

    virtual const QPixmap& GetIcon() const override;

    Path GetVertexShaderFilepath() const;
    Path GetFragmentShaderFilepath() const;

    void Read(const XMLNode &xmlInfo) override;
    void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif
};

#endif // SHADERPROGRAMASSETFILE_H
