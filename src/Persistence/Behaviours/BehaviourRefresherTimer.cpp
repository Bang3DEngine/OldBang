#include "BehaviourRefresherTimer.h"

#include <QThread>

#include "List.h"
#include "Debug.h"
#include "Scene.h"
#include "Application.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManager.h"

#ifdef BANG_EDITOR
#include "EditorState.h"
#include "EditorWindow.h"
#endif

BehaviourRefresherTimer::BehaviourRefresherTimer()
{
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
    List<String> behaviourSources =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    for (String behaviourSource : behaviourSources)
    {
        if (!BehaviourManager::GetStatus().IsReady(behaviourSource))
        {
            BehaviourManager::StartCompilingBehaviourObject(behaviourSource);
        }
    }
}
