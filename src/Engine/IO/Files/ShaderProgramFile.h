#ifndef SHADERPROGRAMASSETFILE_H
#define SHADERPROGRAMASSETFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"

class ShaderProgramFile : public BFile
{
public:
    ShaderProgramFile();
    ShaderProgramFile(const Path& path);

    Path GetVertexShaderFilepath() const;
    Path GetFragmentShaderFilepath() const;

    void Read(const XMLNode &xmlInfo) override;
    void Write(XMLNode *xmlInfo) const override;
};

#endif // SHADERPROGRAMASSETFILE_H
