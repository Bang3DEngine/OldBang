#include "Bang/SystemUtils.h"

#include <QProcess>
#include <QLibrary>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Behaviour.h"
#include "Bang/SingletonManager.h"

List<Path> SystemUtils::GetAllEngineObjects(bool editorMode)
{
    String subdir = editorMode ? "/bin/objEditor" : "/bin/objGame";
    return Path(IO::GetEngineRootAbs() + subdir).GetFiles(true, {"o"});
}

List<Path> SystemUtils::GetAllProjectSubDirs()
{
    Path projectRootPath(IO::GetProjectRootAbs());
    List<Path> subdirs = projectRootPath.GetSubDirectories(true);
    subdirs.PushFront(projectRootPath);
    return subdirs;
}

List<Path> SystemUtils::GetAllEngineSubDirs()
{
    Path engineRootPath(IO::GetEngineRootAbs());
    List<Path> subdirs = engineRootPath.GetSubDirectories(true);
    subdirs.PushFront(engineRootPath);
    return subdirs;
}

List<Path> SystemUtils::GetQtIncludes()
{
    bool ok = false;
    String qtDir = "";
    SystemUtils::System("qmake", {"-query", "QT_INSTALL_HEADERS"}, &qtDir, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt include directories to compile.");
        return {};
    }
    qtDir = qtDir.Replace("\n", "");
    List<Path> incs = Path(qtDir).GetSubDirectories(true);
    for (auto it = incs.Begin(); it != incs.End();)
    {
        const Path& libPath = *it;
        const String& incDir = libPath.GetAbsolute();
        if (!incDir.Contains("qt", false) || !libPath.IsDir())
        {
            incs.Remove(it++);
        }
        else { ++it; }
    }
    incs.PushFront( Path(qtDir) );
    return incs;
}

List<Path> SystemUtils::GetQtLibrariesDirs()
{
    bool ok = false;
    String qtDir = "";
    SystemUtils::System("qmake", {"-query", "QT_INSTALL_LIBS"}, &qtDir, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt library directories to compile.");
        return {};
    }
    qtDir = qtDir.Replace("\n", "");
    List<Path> libs = Path(qtDir).GetSubDirectories(true);
    for (auto it = libs.Begin(); it != libs.End();)
    {
        const Path& libDirPath = *it;
        const String& libDir = libDirPath.GetAbsolute();
        if ( !libDir.Contains("qt", false) || !libDirPath.IsDir() )
        {
            it = libs.Remove(it++);
        }
        else { ++it; }
    }
    libs.PushFront( Path(qtDir) );
    return libs;
}

void SystemUtils::System(const String &command,
                         const List<String> &argsList,
                         String *output,
                         bool *success)
{
    SystemUtils::_System(command, argsList, output, success, false);
}

void SystemUtils::SystemBackground(const String &command,
                                   const List<String> &argsList)
{
    SystemUtils::_System(command, argsList, nullptr, nullptr, true);
}

void SystemUtils::_System(const String &command,
                          const List<String> &argsList,
                          String *output,
                          bool *success,
                          bool background)
{
    QStringList argsListQ;
    for (const String &arg : argsList) { argsListQ.push_back(arg.ToQString()); }

    QProcess process;
    process.setReadChannel(QProcess::ProcessChannel::StandardOutput);
    process.start(command.ToQString(), argsListQ);
    if (!background)
    {
        bool ok = process.waitForFinished(999999);
        ok = ok && (process.exitCode() == 0);

        QString out;
        out = QString( process.readAllStandardOutput() ) +
              QString( process.readAllStandardError() );
        if (output) { *output = String(out) ; }
        if (success) { *success = ok; }
        process.close();
    }
    else
    {
        bool ok = process.startDetached(command.ToQString(), argsListQ);
        if (!ok)
        {
            Debug_Error("Could not start background process " << command);
        }
    }
}

List<String> SystemUtils::ToStringList(const List<Path> &paths)
{
    List<String> stringList;
    for (const Path& path : paths) { stringList.Add(path.GetAbsolute()); }
    return stringList;
}

void SystemUtils::Compile(List<Path> &sourceFilesList,
                          const Path &outputLibFilepath,
                          CompilationFlags clFlags,
                          bool *success,
                          String *output)
{
    typedef CompilationFlag CLFlag;
    const bool editorMode        = !clFlags.IsOn(CLFlag::ForGame);
    const bool addProjObjects    =  clFlags.IsOn(CLFlag::AddProjectObjectFiles);
    const bool addEngineObjects  =  clFlags.IsOn(CLFlag::AddEngineObjectFiles);
    const bool produceSharedLib  =  clFlags.IsOn(CLFlag::ProduceSharedLib);
    const bool addAssetsIncludes =  clFlags.IsOn(CLFlag::AddAssetsIncludeDirs);

    List<String> args;
    args.Add( produceSharedLib ? "-shared" : "-c" );

    List<String> sourceFilesListStr = SystemUtils::ToStringList(sourceFilesList);
    args.Add(sourceFilesListStr);

    args.Add({"-O0", "-g", "-Wl,-O0,--export-dynamic", "-fPIC",
                   "--std=c++11",
                   "-lGLEW", "-lGL", "-lpthread"});
    if (editorMode) { args.Add("-DBANG_EDITOR"); }

    List<Path> qtIncludeDirs = SystemUtils::GetQtIncludes();
    List<String> qtIncludeDirsStr = SystemUtils::ToStringList(qtIncludeDirs);
    for(String &qtIncludeDir : qtIncludeDirsStr) { qtIncludeDir.Prepend("-I"); }
    args.Add(qtIncludeDirsStr);

    args.Add("-I" + IO::GetEngineRootAbs() + "/include");
    if (addAssetsIncludes)
    {
        List<Path> assetsSubDirs =
                   Path(IO::GetProjectAssetsRootAbs()).GetSubDirectories(true);
        List<String> assetsSubDirsStr = SystemUtils::ToStringList(assetsSubDirs);
        for(String &subDir : assetsSubDirsStr) { subDir.Prepend("-I"); }
        args.Add(assetsSubDirsStr);
    }

    List<Path> objectPaths;
    if (addProjObjects)
    {
        Path projectDir(IO::GetProjectRootAbs());
        objectPaths.Add( projectDir.GetFiles(true, {"*.o"}) );
    }

    if (addEngineObjects)
    {
        objectPaths.Add( SystemUtils::GetAllEngineObjects(editorMode) );
    }

    List<String> objectPathsStr = SystemUtils::ToStringList(objectPaths);
    args.Add(objectPathsStr);

    List<Path> qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    List<String> qtLibDirsStr = SystemUtils::ToStringList(qtLibDirs);
    for(String &libDir : qtLibDirsStr) { libDir.Prepend("-L"); }
    args.Add(qtLibDirsStr);

    args.Add({"-o", outputLibFilepath.GetAbsolute()}); // Output to the lib

    Path libsDir = BehaviourManager::GetCurrentLibsDir();
    File::CreateDirectory(libsDir);
    SystemUtils::System("/usr/bin/g++", args, output, success);
}

void SystemUtils::CloseLibrary(QLibrary *library)
{
    if (!library->unload())
    {
        Debug_Error(library->errorString());
    }
}

