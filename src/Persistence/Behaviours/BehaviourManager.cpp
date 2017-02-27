#include "BehaviourManager.h"

#include <QLibrary>

#include "Time.h"
#include "Debug.h"
#include "Scene.h"
#include "Project.h"
#include "StringUtils.h"
#include "Persistence.h"
#include "Application.h"
#include "SystemUtils.h"
#include "SceneManager.h"
#include "ProjectManager.h"
#include "BehaviourHolder.h"
#include "CodePreprocessor.h"

#ifdef BANG_EDITOR
#include "Console.h"
#endif

BehaviourManager::BehaviourManager()
{
}

bool BehaviourManager::SomeBehaviourHasChanged()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    Map<String, String> newBehHashMap = GetBehaviourHashesMap();
    return newBehHashMap != bm->m_behaviourPathToHash;
}

Map<String, String> BehaviourManager::GetBehaviourHashesMap()
{
    Map<String, String> behaviourPathToHash;

    List<String> sourceFilepaths = Persistence::GetFiles(
                Persistence::GetProjectAssetsRootAbs(), true, {"cpp"});
    for (const String &sourceFilepath : sourceFilepaths)
    {
        String hash = GetBehaviourHash(sourceFilepath);
        behaviourPathToHash.Set(sourceFilepath, hash);
    }
    return behaviourPathToHash;
}

QLibrary *BehaviourManager::LoadLibraryFromFilepath(const String &libFilepath)
{
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

void BehaviourManager::RemoveLibraryFiles()
{
    const String libsDir = Persistence::GetProjectLibsRootAbs();
    List<String> libFilepaths = Persistence::GetFiles(libsDir, true);
    for (const String &libFilepath : libFilepaths)
    {
        Persistence::Remove(libFilepath);
    }
}

QLibrary *BehaviourManager::GetBehavioursLibrary()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_behavioursLibrary;
}

void BehaviourManager::RefreshBehavioursLibrary()
{
    Debug_Log("RefreshBehavioursLibrary");
    if (BehaviourManager::GetState() == BehaviourManager::State::Compiling)
    {
        Debug_Log("   Cancelled, currently compiling...");
        return;
    }

    BehaviourManager *bm = BehaviourManager::GetInstance();

    Debug_Log("   SomeBehaviourHasChanged? " << SomeBehaviourHasChanged());
    if (!SomeBehaviourHasChanged()) { return; }

    bm->m_state = State::Compiling;
    bm->m_behaviourPathToHash = GetBehaviourHashesMap();

    RemoveLibraryFiles();

    bm->moveToThread(&bm->m_compileThread);
    QObject::connect(&bm->m_compileThread, SIGNAL(started()),
                     bm, SLOT(CompileBehavioursLibrary()));
    bm->m_compileThread.start();
}

BehaviourManager::State BehaviourManager::GetState()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_state;
}

void BehaviourManager::OnBehavioursLibraryCompiled(const String &libFilepath,
                                                   const String &warnMessage)
{
    if (!warnMessage.Empty()) { Debug_Warn(warnMessage); }

    Debug_Log("OnBehavioursLibraryCompiled " << libFilepath);

    QLibrary *behavioursLib = new QLibrary(libFilepath.ToQString());
    behavioursLib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
    bool success = behavioursLib->load();

    m_behavioursLibrary = success ? behavioursLib : nullptr;
    m_state = success ? State::Success : State::Failed;

    if (!success)
    {
        Debug_Error("There was an error when loading the library '" <<
                     libFilepath << "': " << behavioursLib->errorString());
    }
}

void BehaviourManager::OnBehavioursLibraryCompilationFailed(
        const String &errorMessage)
{
    if (!errorMessage.Empty()) { Debug_Error(errorMessage); }

    m_state = State::Failed;
}

void BehaviourManager::CompileBehavioursLibrary()
{
    Debug_Log("CompileBehavioursLibrary();");

    bool editorMode = false;
    #ifdef BANG_EDITOR
    editorMode = true;
    #endif

    String warnMessage = "", errorMessage = "";

    String includes = " . " + SystemUtils::GetAllProjectSubDirs() + " " +
                              SystemUtils::GetAllEngineSubDirs()  + " " +
                              SystemUtils::GetQtIncludes()        + " ";
    includes.Replace("\n", " ");
    StringUtils::AddInFrontOfWords("-I", &includes);

    String objs = SystemUtils::GetAllProjectObjects()          + " " +
                  SystemUtils::GetAllEngineObjects(editorMode) + " ";

    String qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    qtLibDirs.Replace("\n", " ");
    StringUtils::AddInFrontOfWords("-L", &qtLibDirs);

    String options = " " + objs  + " -O0 -g ";
    if (editorMode) { options += " -DBANG_EDITOR "; }
    options += " -Wl,--export-dynamic --std=c++11 " + includes +
               " -lGLEW -lGL -lpthread " + qtLibDirs + " -fPIC";

    List<String> sources = Persistence::GetFiles(
                Persistence::GetProjectAssetsRootAbs(), true, {"cpp"});
    String sourcesStr = " " + String::Join(sources, " ") + " ";

    String libsDir = Persistence::GetProjectLibsRootAbs();
    Persistence::CreateDirectory(libsDir);
    String libraryFilepath =
            Persistence::GetProjectLibsRootAbs() + "/Behaviours.so";
    libraryFilepath += "." + std::to_string(Time::GetNow()) + ".1.1";

    String cmd = "/usr/bin/g++ -shared " + sourcesStr + " " + options +
                 " -o " + libraryFilepath;
    cmd.Replace("\n", " ");

    String output = "";
    bool successCompiling = false;
    Debug_Log("cmd: " << cmd);
    SystemUtils::System(cmd, &output, &successCompiling);
    Debug_Log("output: " << output);

    if (successCompiling)
    {
        OnBehavioursLibraryCompiled(libraryFilepath.ToQString(),
                                    output.ToQString());
    }
    else
    {
        OnBehavioursLibraryCompilationFailed(output.ToQString());
    }


    QObject::disconnect(&m_compileThread, SIGNAL(started()),
                        this, SLOT(CompileBehavioursLibrary()));
    m_compileThread.quit();
}

String BehaviourManager::GetBehaviourHash(const String &behaviourFilepath)
{
    String code = Persistence::GetFileContents(behaviourFilepath);

    List<String> includePaths = Persistence::GetSubDirectories(
                Persistence::GetProjectAssetsRootAbs(), true);
    CodePreprocessor::PreprocessCode(&code, includePaths);

    return Persistence::GetHashFromString(code);
}
