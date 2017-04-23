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

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual SerializableObject *GetNewInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    bool IsTriangles() const;
    int GetNumFaces() const;
};

#endif // FILEMESH_H
