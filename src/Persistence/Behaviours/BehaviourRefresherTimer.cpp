#include "Bang/BehaviourRefresherTimer.h"

#include <QThread>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"

#ifdef BANG_EDITOR
#include "Bang/EditorState.h"
#include "Bang/EditorWindow.h"
#endif

BehaviourRefresherTimer::BehaviourRefresherTimer()
{
    c_timeMs = 2000;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnRefreshTimer()));
    m_timer.start(c_timeMs);
}

void BehaviourRefresherTimer::OnRefreshTimer() const
{
    #ifdef BANG_EDITOR
    if (IWindow::GetInstance()->IsInFront())
    {
        RefreshBehaviours();
    }
    #endif
}

void BehaviourRefresherTimer::RefreshBehaviours() const
{
    BehaviourManager::SetCurrentLibsDir(Paths::ProjectLibraries());

    const List<Path>& behaviourSources =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    for (const Path &behaviourSource : behaviourSources)
    {
        if (!BehaviourManager::GetStatus().IsReady(behaviourSource))
        {
            BehaviourManager::StartCompilingBehaviourObject(
                        behaviourSource, false);
        }
    }
}
