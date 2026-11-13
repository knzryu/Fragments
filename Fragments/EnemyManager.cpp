#include "DxLib.h"
#include "EnemyManager.h"
#include <cstdio>
// 敵の初期生成
void EnemyManager::Initialize(const CourseManager& courseManager)
{
    Clear();

    SpawnEnemy(20.0f, -3.0f, 0.0f, courseManager);
    SpawnEnemy(30.0f, 3.0f, 0.0f, courseManager);
}

// 敵の生成
void EnemyManager::SpawnEnemy(
    float courseZ,
    float laneOffset,
    float y,
    const CourseManager& courseManager)
{
    Enemy enemy;

    // コース上の位置を設定
    enemy.courseZ = courseZ;
    enemy.laneOffset = laneOffset;
    enemy.y = y;

    DrawFormatString(
        20, 180,
        GetColor(255, 255, 0),
        "Spawn input: %.2f / stored: %.2f",
        courseZ,
        enemy.courseZ
    );

    // ワールド座標を計算
    enemy.UpdatePosition(courseManager);

    // 管理配列に追加
    enemies.push_back(enemy);

    printf(
        "SpawnEnemy: courseZ=%.2f, laneOffset=%.2f\n",
        enemy.courseZ,
        enemy.laneOffset
    );
}




// 敵の描画 
void EnemyManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (enemies[i].IsDead())
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