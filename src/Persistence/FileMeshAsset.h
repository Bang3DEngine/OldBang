#ifndef FILEMESHASSET_H
#define FILEMESHASSET_H

#include "FileReader.h"
#include "Logger.h"

#include "File.h"

class FileMeshAsset : public File
{
    std::string m_meshFilepath = "";

public:
    FileMeshAsset(const QFileSystemModel *model, const QModelIndex *index);
    const std::string& GetMeshFilepath() const;
};

#endif // FILEMESHASSET_H
