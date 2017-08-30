#include "Bang/File.h"

#include <cstdio>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

File::~File()
{
}

bool File::DuplicateFile(const Path &fromFilepath,
                         const Path &toFilepath,
                         bool overwrite)
{
    if (!fromFilepath.IsFile()) { return false; }
    if (!overwrite && toFilepath.Exists()) { return false; }

    std::ifstream src(fromFilepath.GetAbsolute().ToCString(), std::ios::binary);
    if (!src) { return false; }

    if (overwrite) { File::Remove(toFilepath); }
    std::ofstream dst(toFilepath.GetAbsolute().ToCString(),   std::ios::binary);
    if (!dst) { return false; }

    dst << src.rdbuf();
    return true;
}

bool File::DuplicateDir(const Path &fromDirpath,
                        const Path &toDirpath,
                        bool overwrite)
{
    if (!fromDirpath.IsDir()) { return false; }
    if (!overwrite && toDirpath.Exists()) { return false; }
    if (!File::CreateDirectory(toDirpath)) { return false; }

    List<Path> filepaths = fromDirpath.FindFiles(Path::FindFlag::SimpleHidden);
    for(const Path& filepath : filepaths)
    {
        String fileName = filepath.GetNameExt();
        bool ok = File::DuplicateFile(fromDirpath.Append(fileName),
                                      toDirpath.Append(fileName),
                                      overwrite);
        if (!ok) { return false; }
    }

    List<Path> subdirs = fromDirpath.FindSubDirectories(Path::FindFlag::SimpleHidden);
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
        return std::remove(path.GetAbsolute().ToCString()) == 0;
    }
    else
    {
        List<Path> subDirs  = path.FindSubDirectories(Path::FindFlag::SimpleHidden);
        for (const Path &subDir : subDirs) { File::Remove(subDir); }

        List<Path> subFiles = path.FindFiles(Path::FindFlag::SimpleHidden);
        for (const Path &subFile : subFiles) { File::Remove(subFile); }
        return std::remove(path.GetAbsolute().ToCString()) == 0;
    }
}

bool File::CreateDirectory(const Path &dirPath)
{
    if (dirPath.Exists()) { return true; }
    return mkdir(dirPath.GetAbsolute().ToCString(), 0700) == 0;
}

bool File::Rename(const Path &oldPath, const Path &newPath)
{
    if (!oldPath.Exists()) { return false; }
    return rename(oldPath.GetAbsolute().ToCString(),
                  newPath.GetAbsolute().ToCString());
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

String File::GetContents() const
{
    return File::GetContents( GetPath() );
}

const Path &File::GetPath() const
{
    return m_path;
}
