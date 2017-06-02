#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include <QCryptographicHash>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/String.h"

class File
{
public:
    File();
    File(const Path &filepath);
    File(const String &filepath);
    File(const QFileSystemModel *model, const QModelIndex &index);
    virtual ~File();

    String GetContents() const;
    const Path& GetPath() const;

    static bool Remove(const Path &path);
    static bool CreateDirectory(const Path &dirPath);
    static bool Move(const Path &oldPath, const Path &newPath);

    static bool Duplicate(const Path &fromPath, const Path &toPath);
    static bool DuplicateFile(const Path &fromFilepath,
                              const Path &toFilepath,
                              bool overwrite = true);
    static bool DuplicateDir(const Path &fromDirpath,
                             const Path &toDirpath,
                             bool overwrite = true);

    static String GetContents(const Path &filepath);
    static void Write(const Path &filepath, const String &contents);
    static void Write(const Path &filepath, const Array<String> &lines);
    static void Write(const Path &filepath, const List<String> &lines);

    static String GetHash(const Path &filepath);
    static String GetHashFromString(const String &str);
    static String GetHashFromByteArray(const QByteArray &byteArray);

protected:
    Path m_path;
};

#endif // FILE_H
