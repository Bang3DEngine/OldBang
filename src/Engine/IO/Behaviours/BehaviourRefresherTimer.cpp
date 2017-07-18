#include "Bang/BehaviourRefresherTimer.h"

#include <QThread>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/EditorState.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"

BehaviourRefresherTimer::BehaviourRefresherTimer()
{
    c_timeMs = 500;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnRefreshTimer()));
    m_timer.start(c_timeMs);
}

void BehaviourRefresherTimer::OnRefreshTimer() const
{
    const Path libsDir = Paths::ProjectLibrariesDir();

    Window *win = Window::GetInstance();
    if (win && win->IsInFront())
    {
        BehaviourManager::StartCompilingAllBehaviourObjects(false, libsDir);
    }
}
