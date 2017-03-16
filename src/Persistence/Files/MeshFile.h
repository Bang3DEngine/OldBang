#ifndef FILEMESH_H
#define FILEMESH_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "File.h"

class SerializableObject;
class MeshFile : public File
{
    bool m_isTris = true;
    bool m_hasUvs = false;
    bool m_hasNormals = false;
    int m_numFaces = 0;

public:
    MeshFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual SerializableObject *GetInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    bool IsTriangles() const;
    bool HasUvs() const;
    bool HasNormals() const;
    int GetNumFaces() const;
};

#endif // FILEMESH_H
