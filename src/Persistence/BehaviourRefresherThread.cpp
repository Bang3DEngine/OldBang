#include "BehaviourRefresherThread.h"

#include "List.h"
#include "Debug.h"
#include "Scene.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManager.h"

BehaviourRefresherThread::BehaviourRefresherThread()
{
}

void BehaviourRefresherThread::run()
{
    while (true)
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
                bh->Refresh();
            }
        }

        QThread::sleep(c_RefreshDelaySeconds);
    }
}
