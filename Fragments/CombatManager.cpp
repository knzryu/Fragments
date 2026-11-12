#include "CombatManager.h"
#include "Enemy.h"
#include "Collision.h"

// “G‚Æ’e‚Ìí“¬ˆ—
bool CombatManager::Update(
    EnemyManager& enemyManager,
    BulletManager& bulletManager,
    PlayerProgress& progress)
{
    bool isLevelUp = false;

    // ‚·‚×‚Ä‚Ì“G‚ğŠm”F
    for (int e = 0; e < enemyManager.GetCount(); e++)
    {
        Enemy& enemy = enemyManager.GetEnemy(e);

        // “|‚³‚ê‚½“G‚Í”»’è‚µ‚È‚¢
        if (enemy.hp <= 0)
        {
            continue;
        }

        // “G‚Ì“–‚½‚è”»’è”ÍˆÍ
        float enemyMinX = enemy.x - 0.5f;
        float enemyMaxX = enemy.x + 0.5f;
        float enemyMinY = enemy.y;
        float enemyMaxY = enemy.y + 2.0f;
        float enemyMinZ = enemy.z - 0.5f;
        float enemyMaxZ = enemy.z + 0.5f;

        // ‚·‚×‚Ä‚Ì’e‚ğŒã‚ë‚©‚çŠm”F
        for (int i = bulletManager.GetCount() - 1; i >= 0; i--)
        {
            auto& bullet = bulletManager.GetBullet(i);

            // –³Œø‚È’e‚Í”»’è‚µ‚È‚¢
            if (!bullet.active)
            {
                continue;
            }

            // ’e‚Ì“–‚½‚è”»’è”ÍˆÍ
            float bulletMinX = bullet.x - 0.15f;
            float bulletMaxX = bullet.x + 0.15f;
            float bulletMinY = bullet.y - 0.15f;
            float bulletMaxY = bullet.y + 0.15f;
            float bulletMinZ = bullet.z - 0.15f;
            float bulletMaxZ = bullet.z + 0.15f;

            // “G‚Æ’e‚ÌÕ“Ë”»’è
            bool isHit = IsCollidingAABB(
                bulletMinX, bulletMaxX,
                bulletMinY, bulletMaxY,
                bulletMinZ, bulletMaxZ,
                enemyMinX, enemyMaxX,
                enemyMinY, enemyMaxY,
                enemyMinZ, enemyMaxZ
            );

            if (isHit)
            {
                // ƒ_ƒ[ƒW‚ğ—^‚¦‚é‘O‚ÌHP
                int previousHP = enemy.hp;

                // ƒ_ƒ[ƒW‚ğ—^‚¦‚é
                enemy.TakeDamage(1);

                // “G‚ğ“|‚µ‚½uŠÔ‚¾‚¯•ñV‚ğ—^‚¦‚é
                if (previousHP > 0 && enemy.hp == 0)
                {
                    int previousLevel = progress.GetPlayerLevel();

                    // Œ‚”j”‚ğ‘‚â‚·
                    progress.AddDefeatedEnemyCount();

                    // ŒoŒ±’l‚ğ’Ç‰Á
                    progress.AddExperience(10);

                    // ƒŒƒxƒ‹ƒAƒbƒv‚ğŠm”F
                    if (progress.GetPlayerLevel() > previousLevel)
                    {
                        isLevelUp = true;
                    }
                }

                // –½’†‚µ‚½’e‚ğ–³Œø‰»
                bullet.active = false;

                // “G‚ğ“|‚µ‚½‚çc‚è‚Ì’e‚Æ‚Ì”»’è‚ğI—¹
                if (enemy.hp <= 0)
                {
                    break;
                }
            }
        }
    }

    // –³Œø‚È’e‚ğíœ
    bulletManager.RemoveInactiveBullets();

    return isLevelUp;
}