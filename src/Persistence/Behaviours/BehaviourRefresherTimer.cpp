#include "BehaviourRefresherTimer.h"

#include <QThread>

#include "List.h"
#include "Debug.h"
#include "Scene.h"
#include "EditorState.h"
#include "Application.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManager.h"

BehaviourRefresherTimer::BehaviourRefresherTimer()
{
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(OnRefreshTimer()));
    m_timer.start(c_timeMs);
}

void BehaviourRefresherTimer::OnRefreshTimer() const
{
    RefreshBehavioursInScene();
}

void BehaviourRefresherTimer::RefreshBehavioursInScene() const
{
    if (EditorState::IsPlaying()) { return; }

    BehaviourManager *bManager = BehaviourManager::GetInstance();
    Scene *scene = SceneManager::GetActiveScene();

    bool canRefresh = (bManager && scene);
    if (canRefresh)
    {
        // Look for all behaviour holders in the scene and update its scripts
        List<BehaviourHolder*> behHolders =
                scene->GetComponentsInThisAndChildren<BehaviourHolder>();
        for (BehaviourHolder *bh : behHolders)
        {
            bh->Refresh();
        }
    }
}
