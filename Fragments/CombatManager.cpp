#include "CombatManager.h"
#include "Enemy.h"
#include "Collision.h"


// クールタイム初期化
void CombatManager::Initialize()
{
    shotTimer = 0.0f;
}

// クールタイム更新
void CombatManager::UpdateCooldown(float deltaTime)
{
    if (shotTimer > 0.0f)
    {
        shotTimer -= deltaTime;

        if (shotTimer < 0.0f)
        {
            shotTimer = 0.0f;
        }
    }
}

// 発射可能か確認
bool CombatManager::CanShot() const
{
    return shotTimer <= 0.0f;
}

// クールタイム開始
void CombatManager::StartCooldown()
{
    shotTimer = shotCooldown;
}

// 残り時間取得
float CombatManager::GetShotTimer() const
{
    return shotTimer;
}

// 敵と弾の戦闘処理
bool CombatManager::Update(
    EnemyManager& enemyManager,
    BulletManager& bulletManager,
    PlayerProgress& progress)
{
    bool isLevelUp = false;

    // すべての敵を確認
    for (int e = 0; e < enemyManager.GetCount(); e++)
    {
        Enemy& enemy = enemyManager.GetEnemy(e);

        // 倒された敵は判定しない
        if (enemy.IsDead())
        {
            continue;
        }

        // 敵の当たり判定範囲
        float enemyMinX = enemy.x - 0.5f;
        float enemyMaxX = enemy.x + 0.5f;
        float enemyMinY = enemy.y;
        float enemyMaxY = enemy.y + 2.0f;
        float enemyMinZ = enemy.z - 0.5f;
        float enemyMaxZ = enemy.z + 0.5f;

        // すべての弾を後ろから確認
        for (int i = bulletManager.GetCount() - 1; i >= 0; i--)
        {
            auto& bullet = bulletManager.GetBullet(i);

            // 無効な弾は判定しない
            if (!bullet.active)
            {
                continue;
            }

            // 弾の当たり判定範囲
            float bulletMinX = bullet.x - 0.15f;
            float bulletMaxX = bullet.x + 0.15f;
            float bulletMinY = bullet.y - 0.15f;
            float bulletMaxY = bullet.y + 0.15f;
            float bulletMinZ = bullet.z - 0.15f;
            float bulletMaxZ = bullet.z + 0.15f;

            // 敵と弾の衝突判定
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
                // ダメージを与える前のHP
                int previousHP = enemy.GetHP();

                // ダメージを与える
                enemy.TakeDamage(1);

                // 敵を倒した瞬間だけ報酬を与える
                if (previousHP > 0 && enemy.IsDead())
                {
                    int previousLevel = progress.GetPlayerLevel();

                    // 撃破数を増やす
                    progress.AddDefeatedEnemyCount();

                    // 経験値を追加
                    progress.AddExperience(10);

                    // レベルアップを確認
                    if (progress.GetPlayerLevel() > previousLevel)
                    {
                        isLevelUp = true;
                    }
                }

                // 命中した弾を無効化
                bullet.active = false;

                // 敵を倒したら残りの弾との判定を終了
                if (enemy.IsDead())
                {
                    break;
                }
            }
        }
    }

    // 無効な弾を削除
    bulletManager.RemoveInactiveBullets();

    return isLevelUp;
}