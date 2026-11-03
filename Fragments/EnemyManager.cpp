#include "EnemyManager.h"

int testValue = 0;
// 敵の初期生成
void EnemyManager::Initialize()
{
    Clear();

    SpawnEnemy(-3.0f, 0.0f, 20.0f);
    SpawnEnemy(3.0f, 0.0f, 30.0f);
}

// 敵の生成
void EnemyManager::SpawnEnemy(float x, float y, float z)
{
    Enemy enemy;
    enemy.Initialize(x, y, z);

    enemies.push_back(enemy);
}

// 敵の描画
void EnemyManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (enemies[i].hp > 0)
        {
            enemies[i].Draw();
        }
    }
}

// 敵の数を取得　
int EnemyManager::GetCount() const
{
    return static_cast<int>(enemies.size());
}

// 敵を取得　
Enemy& EnemyManager::GetEnemy(int index) 
{
    return enemies[index];
}

// 敵を全削除
void EnemyManager::Clear()
{
    enemies.clear();
}