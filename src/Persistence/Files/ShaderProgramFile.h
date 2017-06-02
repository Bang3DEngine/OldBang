#ifndef SHADERPROGRAMASSETFILE_H
#define SHADERPROGRAMASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"
#include "Bang/IInspectable.h"

class ShaderProgramFile : public BFile
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
