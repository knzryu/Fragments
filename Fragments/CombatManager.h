#include "EnemyManager.h"
#include "BulletManager.h"
#include "PlayerProgress.h"

class CombatManager
{
public:

    // “G‚Æ’e‚Ìí“¬ˆ—
    bool Update(
        EnemyManager& enemyManager,
        BulletManager& bulletManager,
        PlayerProgress& progress
    );
};