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

bool IO::IsEngineFile(const String &filepath)
{
    if (filepath.Empty()) { return false; }
    return filepath.BeginsWith(IO::GetEngineAssetsRootAbs());
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



