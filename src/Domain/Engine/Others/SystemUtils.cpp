#include "Bang/SystemUtils.h"

#include <QProcess>
#include <QLibrary>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Behaviour.h"
#include "Bang/BehaviourHolder.h"
#include "Bang/SingletonManager.h"

List<String> SystemUtils::GetAllEngineObjects(bool editorMode)
{
    String subdir = editorMode ? "/bin/objEditor" : "/bin/objGame";
    return IO::GetFiles(IO::GetEngineRootAbs() + subdir, true, {"*.o"});
}

List<String> SystemUtils::GetAllProjectSubDirs()
{
    List<String> subdirs = IO::GetSubDirectories(IO::GetProjectRootAbs(), true);
    subdirs.PushFront(IO::GetProjectRootAbs());
    return subdirs;
}

List<String> SystemUtils::GetAllEngineSubDirs()
{
    List<String> subdirs = IO::GetSubDirectories(IO::GetEngineRootAbs(), true);
    subdirs.PushFront(IO::GetEngineRootAbs());
    return subdirs;
}

List<String> SystemUtils::GetQtIncludes()
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
    List<String> incs = IO::GetSubDirectories(qtDir, true);
    for (auto it = incs.Begin(); it != incs.End();)
    {
        const String& incDir = *it;
        if (!incDir.Contains("qt", false) || !IO::IsDir(incDir))
        {
            incs.Remove(it++);
        }
        else { ++it; }
    }
    incs.PushFront(qtDir);
    return incs;
}

List<String> SystemUtils::GetQtLibrariesDirs()
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
    List<String> libs = IO::GetSubDirectories(qtDir, true);
    for (auto it = libs.Begin(); it != libs.End();)
    {
        const String& libDir = *it;
        if ( !libDir.Contains("qt", false) || !IO::IsDir(libDir) )
        {
            it = libs.Remove(it++);
        }
        else { ++it; }
    }
    libs.PushFront(qtDir);
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

void SystemUtils::Compile(List<String> &sourceFilesList,
                          const String &outputLibFilepath,
                          CompilationFlags clFlags,
                          bool *success, String *output)
{
    typedef CompilationFlags CLFlags;
    const bool editorMode = (clFlags & CLFlags::ForGame) == 0;
    const bool addProjObjects = (clFlags & CLFlags::AddProjectObjectFiles) > 0;
    const bool addEngineObjects = (clFlags & CLFlags::AddEngineObjectFiles) > 0;
    const bool produceSharedLib = (clFlags & CLFlags::ProduceSharedLib) > 0;

    List<String> args = {};

    args.PushBack( produceSharedLib ? "-shared" : "-c" );
    args.PushBack(sourceFilesList);
    args.PushBack({"-O0", "-g", "-Wl,-O0,--export-dynamic", "-fPIC",
                   "--std=c++11",
                   "-lGLEW", "-lGL", "-lpthread"});
    if (editorMode) { args.PushBack("-DBANG_EDITOR"); }

    {
    args.PushBack("-Iinclude/Bang");
    }

    {
    List<String> objects;
    if (addProjObjects)
    {
        String dir = IO::GetDir(outputLibFilepath);
        objects.PushBack(IO::GetFiles(dir, true, {"*.o"}));
    }
    if (addEngineObjects)
    {
        objects.PushBack(SystemUtils::GetAllEngineObjects(editorMode));
    }
    args.PushBack(objects);
    }

    {
    List<String> qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    for(String &libDir : qtLibDirs) { libDir.Prepend("-L"); }
    args.PushBack(qtLibDirs);
    }

    args.PushBack({"-o", outputLibFilepath});

    String libsDir = BehaviourManager::GetCurrentLibsDir();
    IO::CreateDirectory(libsDir);
    SystemUtils::System("/usr/bin/g++", args, output, success);
}

void SystemUtils::CloseLibrary(QLibrary *library)
{
    if (!library->unload())
    {
        Debug_Error(library->errorString());
    }
}

