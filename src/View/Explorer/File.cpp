#include "File.h"

File::File(const QFileSystemModel *model, const QModelIndex *index)
{
    isFile = !model->isDir(*index);

    path = model->filePath(*index).toStdString();
    name = model->fileName(*index).toStdString();

    std::string::size_type p = name.find('.');
    if(p != std::string::npos)
    {
        extension = name.substr(p+1, name.length() - p - 1);
        name = name.substr(0, p);
    }
}

bool File::IsImage() const
{
    return  isFile &&
            (
            extension == "jpg" || extension == "jpeg" ||
            extension == "png" || extension == "bmp"
            );
}

bool File::IsDir() const
{
    return !isFile;
}

bool File::IsFile() const
{
    return isFile;
}

const std::string &File::GetName() const
{
    return name;
}

const std::string &File::GetPath() const
{
    return path;
}

const std::string &File::GetExtension() const
{
    return extension;
}
