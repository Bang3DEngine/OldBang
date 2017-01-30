#include "BehaviourRefresherTimer.h"

#include "List.h"
#include "Debug.h"
#include "Scene.h"
#include "Application.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManager.h"

BehaviourRefresherTimer::BehaviourRefresherTimer()
{
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(OnRefreshTimer()));
    m_timer.start(3000);
}

void BehaviourRefresherTimer::OnRefreshTimer() const
{
    RefreshBehavioursInScene(false);
}

void BehaviourRefresherTimer::RefreshBehavioursInScene(bool synchronously) const
{
    BehaviourManager *bManager = BehaviourManager::GetInstance();
    Scene *scene = SceneManager::GetActiveScene();

    bool canUpdate = (bManager && scene);
    if (canUpdate)
    {
        // Look for all behaviour holders in the scene and update its scripts
        List<BehaviourHolder*> behHolders =
                scene->GetComponentsInThisAndChildren<BehaviourHolder>();
        for (BehaviourHolder *bh : behHolders)
        {
            bh->Refresh(synchronously);
            if (synchronously)
            {
                // If its sync, let it update the gui between
                // compilation and compilation...
                Application::GetInstance()->processEvents();
            }
        }
    }
}
