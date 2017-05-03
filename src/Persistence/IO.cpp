#include "Bang/IO.h"

#include <QDir>
#include <QFile>
#include <ostream>
#include <fstream>
#include <QCryptographicHash>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/SingletonManager.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#endif

bool IO::IsDir(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isDir();
}

bool IO::IsFile(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isFile();
}

bool IO::IsAbsolute(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isAbsolute();
}

String IO::GetDir(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String directory = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        directory = filepath.substr(0, lastSlash);
    }
    return directory;
}

String IO::GetBaseName(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String filename = filepath;
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    Array<String> parts = filename.Split('.');
    if (!parts.Empty())
    {
        filename = parts[0];
    }

    return filename;
}

String IO::GetFileNameWithExtension(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String filename = filepath;
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    return filename;
}

String IO::GetFileExtensionFirst(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[1];
}

String IO::GetFileExtensionLast(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[parts.Size() - 1];
}

String IO::GetFileContents(const Path &filepath)
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
        Debug_Error("Can't open file '" << filepath << "': " << std::strerror(errno));
    }
    return contents;
}

String IO::GetFileExtensionComplete(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    parts.Remove(parts.Begin());
    return String::Join(parts, ".");
}

String IO::GetPathWithoutExtension(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String path = GetDir(filepath);
    path += GetBaseName(filepath);
    return path;
}

bool IO::IsEngineFile(const String &filepath)
{
    if (filepath.Empty()) { return false; }
    return filepath.BeginsWith(IO::GetEngineAssetsRootAbs());
}

bool IO::DuplicateFile(const Path &fromFilepath,
                       const Path &toFilepath,
                       bool overwrite)
{
    if (!fromFilepath.IsFile()) { return false; }
    if (overwrite) { IO::Remove(toFilepath); }
    bool ok = QFile::copy(fromFilepath.GetAbsolute().ToQString(),
                          toFilepath.GetAbsolute().ToQString());
    return ok;
}

bool IO::DuplicateDir(const Path &fromDirpath,
                      const Path &toDirpath,
                      bool overwrite)
{
    if (!fromDirpath.IsDir()) { return false; }

    if (!IO::CreateDirectory( Path(toDirpath) )) { return false; }
    List<Path> filepaths = fromDirpath.GetFiles(false);
    for(const Path& filepath : filepaths)
    {
        String fileName = filepath.GetNameExt();
        bool ok = IO::DuplicateFile(fromDirpath.Append(fileName),
                                    toDirpath.Append(fileName),
                                    overwrite);
        if (!ok) { return false; }
    }

    List<Path> subdirs = fromDirpath.GetSubDirectories(false);
    for (const Path &subdir : subdirs)
    {
        bool ok = IO::DuplicateDir(subdir,
                                   toDirpath.Append(subdir.GetName()),
                                   overwrite);
        if (!ok) { return false; }
    }
    return true;
}

bool IO::Remove(const Path &path)
{
    if (!path.Exists()) { return false; }
    if (path.IsFile())
    {
        QFile f(path.GetAbsolute().ToQString());
        return f.remove();
    }
    else
    {
        List<Path> subDirs  = path.GetSubDirectories(false);
        for (const Path &subDir : subDirs) { IO::Remove(subDir); }
        List<Path> subFiles = path.GetFiles(false);
        for (const Path &subFile : subFiles) { IO::Remove(subFile); }
        QDir().rmdir(path.GetAbsolute().ToQString());
    }
    return false;
}

bool IO::CreateDirectory(const Path &dirPath)
{
    if (dirPath.IsEmpty()) { return false; }
    if (dirPath.IsDir()) { return true; }
    return QDir().mkdir(dirPath.GetAbsolute().ToQString());
}

bool IO::Rename(const Path &oldPath, const Path &newPath)
{
    return IO::Move(oldPath, newPath);
}

bool IO::Move(const Path &oldPath, const Path &newPath)
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

bool IO::WriteToFile(const Path &filepath, const String &contents)
{
    std::ofstream out(filepath.GetAbsolute());
    if (out.is_open())
    {
        out << contents;
        out.close();
        return true;
    }
    return false;
}

bool IO::IsEngineFile(const Path &path)
{
    return path.GetAbsolute().BeginsWith( IO::GetEngineAssetsRootAbs() );
}

String IO::GetHash(const Path &filepath)
{
    if (!filepath.IsFile()) { return ""; }

    String result = "";
    QFile file(filepath.GetAbsolute().ToQString());
    if(file.open(QIODevice::ReadOnly))
    {
        result = IO::GetHashFromByteArray(file.readAll());
        file.close();
    }
    return result;
}

String IO::GetHashFromString(const String &str)
{
    return IO::GetHashFromByteArray( str.ToQString().toUtf8() );
}

String IO::GetHashFromByteArray(const QByteArray &byteArray)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QByteArray hashBytes = hash.hash(byteArray, QCryptographicHash::Sha1);
    return String( QString(hashBytes.toHex()) );
}

String IO::AppendExtension(const String &filepath, const String extNoDot)
{
	if (filepath.Empty()) { return ""; }
    if (filepath.EndsWith("." + extNoDot)) { return filepath; }
    return filepath + "." + extNoDot;
}

void IO::InitFromMainBinary()
{
    SingletonManager::Set<IO>(new IO());
}

IO *IO::GetInstance()
{
    return SingletonManager::Get<IO>();
}

const String &IO::GetProjectRootAbs()
{
    return IO::GetInstance()->c_ProjectRootAbsolute;
}

String IO::GetProjectLibsRootAbs()
{
    return IO::GetInstance()->c_ProjectRootAbsolute + "/Libraries";
}
const String &IO::GetProjectAssetsRootAbs()
{
    return IO::GetInstance()->c_ProjectAssetsRootAbsolute;
}
const String &IO::GetEngineRootAbs()
{
    return IO::GetInstance()->c_EngineRootAbsolute;
}
const String &IO::GetEngineAssetsRootAbs()
{
    return IO::GetInstance()->c_EngineAssetsRootAbsolute;
}



