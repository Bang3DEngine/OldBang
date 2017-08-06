#include "Bang/File.h"

#include <fstream>
#include <iostream>

#include <QModelIndex>
#include <QFileSystemModel>
#include <QCryptographicHash>

#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/String.h"

File::File()
{
}


File::File(const Path &filepath)
{
    m_path = filepath;
}

File::File(const String &filepath) : File( Path(filepath) )
{
}

File::File(const QFileSystemModel *model, const QModelIndex &index)
{
    String pathStr(model->filePath(index).toStdString());
    m_path = Path(pathStr);
}

File::~File()
{
}

bool File::DuplicateFile(const Path &fromFilepath,
                             const Path &toFilepath,
                             bool overwrite)
{
    if (!fromFilepath.IsFile()) { return false; }
    if (overwrite) { File::Remove(toFilepath); }
    bool ok = QFile::copy(fromFilepath.GetAbsolute().ToQString(),
                          toFilepath.GetAbsolute().ToQString());
    return ok;
}

bool File::DuplicateDir(const Path &fromDirpath,
                            const Path &toDirpath,
                            bool overwrite)
{
    if (!fromDirpath.IsDir()) { return false; }

    if (!File::CreateDirectory( Path(toDirpath) )) { return false; }
    List<Path> filepaths = fromDirpath.FindFiles(false);
    for(const Path& filepath : filepaths)
    {
        String fileName = filepath.GetNameExt();
        bool ok = File::DuplicateFile(fromDirpath.Append(fileName),
                                          toDirpath.Append(fileName),
                                          overwrite);
        if (!ok) { return false; }
    }

    List<Path> subdirs = fromDirpath.FindSubDirectories(false);
    for (const Path &subdir : subdirs)
    {
        bool ok = File::DuplicateDir(subdir,
                                   toDirpath.Append(subdir.GetName()),
                                   overwrite);
        if (!ok) { return false; }
    }
    return true;
}

bool File::Remove(const Path &path)
{
    if (!path.Exists()) { return false; }
    if (path.IsFile())
    {
        QFile f(path.GetAbsolute().ToQString());
        return f.remove();
    }
    else
    {
        List<Path> subDirs  = path.FindSubDirectories(false);
        for (const Path &subDir : subDirs) { File::Remove(subDir); }
        List<Path> subFiles = path.FindFiles(false);
        for (const Path &subFile : subFiles) { File::Remove(subFile); }
        QDir().rmdir(path.GetAbsolute().ToQString());
    }
    return false;
}

bool File::CreateDirectory(const Path &dirPath)
{
    if (dirPath.IsEmpty()) { return false; }
    if (dirPath.IsDir()) { return true; }
    return QDir().mkdir(dirPath.GetAbsolute().ToQString());
}

bool File::Move(const Path &oldPath, const Path &newPath)
{
    if (!oldPath.Exists()) { return false; }
    if (oldPath.IsDir())
    {
        return QDir().rename(oldPath.GetAbsolute().ToQString(),
                             newPath.GetAbsolute().ToQString());
    }
    return QFile().rename(oldPath.GetAbsolute().ToQString(),
                          newPath.GetAbsolute().ToQString());
}

bool File::Duplicate(const Path &fromPath, const Path &toPath)
{
    if (fromPath.IsFile())
    {
        return File::DuplicateFile(fromPath, toPath);
    }
    else if (fromPath.IsDir())
    {
        return File::DuplicateDir(fromPath, toPath);
    }
    return false;
}

void File::Write(const Path &filepath, const String &contents)
{
    std::ofstream out(filepath.GetAbsolute());
    if (out)
    {
        out << contents;
        out.close();
    }
}

void File::Write(const Path &filepath, const Array<String> &lines)
{
    File::Write(filepath, String::Join(lines, "\n"));
}

void File::Write(const Path &filepath, const List<String> &lines)
{
    File::Write(filepath, String::Join(lines, "\n"));
}

String File::GetContents(const Path &filepath)
{
    if (!filepath.IsFile()) { return ""; }

    String contents = "";
    std::ifstream ifs(filepath.GetAbsolute().ToCString());
    if (ifs)
    {
        contents = String((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
        ifs.close();
    }
    else
    {
        std::cerr << "Can't open file '" << filepath.ToString() << "': " <<
                     std::strerror(errno) << std::endl;
    }
    return contents;
}

String File::GetHash(const Path &filepath)
{
    if (!filepath.IsFile()) { return ""; }

    String result = "";
    QFile file(filepath.GetAbsolute().ToQString());
    if(file.open(QIODevice::ReadOnly))
    {
        result = File::GetHashFromByteArray(file.readAll());
        file.close();
    }
    return result;
}

String File::GetHashFromString(const String &str)
{
    return File::GetHashFromByteArray( str.ToQString().toUtf8() );
}

String File::GetHashFromByteArray(const QByteArray &byteArray)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QByteArray hashBytes = hash.hash(byteArray, QCryptographicHash::Sha1);
    return String( QString(hashBytes.toHex()) );
}

String File::GetContents() const
{
    return File::GetContents( GetPath() );
}

const Path &File::GetPath() const
{
    return m_path;
}
