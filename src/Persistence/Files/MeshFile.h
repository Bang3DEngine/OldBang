#ifndef FILEMESH_H
#define FILEMESH_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

class SerializableObject;
class MeshFile : public File
{
    bool m_isTris = true;
    int m_numFaces = 0;

public:
    MeshFile();
    MeshFile(const QFileSystemModel *model, const QModelIndex &index);

    void Read(const XMLNode &xmlInfo) override;
    void Write(XMLNode *xmlInfo) const override;

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

    bool IsTriangles() const;
    int GetNumFaces() const;
};

#endif // FILEMESH_H
