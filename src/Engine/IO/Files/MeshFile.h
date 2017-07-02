#ifndef MESHFILE_H
#define MESHFILE_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/FileReader.h"
#include "Bang/IInspectable.h"

class SerializableObject;
class MeshFile : public BFile
{
public:
    MeshFile();
    MeshFile(const Path& path);
    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

    void SetModelFilepath(const Path &modelFilepath);
    const Path& GetModelFilepath() const;

    void Read(const XMLNode &xmlInfo);
    void Write(XMLNode *xmlInfo) const;

private:
    Path m_modelFilepath;
};

#endif // MESHASSETFILE_H
