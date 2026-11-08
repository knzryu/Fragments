#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include "Enemy.h"
#include "CourseManager.h"

class EnemyManager
{
private:
    std::vector<Enemy> enemies;

public:
    // 敵の初期生成
    void Initialize(const CourseManager& courseManager);

    // 敵の生成
    void SpawnEnemy(
        float courseZ,
        float laneOffset,
        float y,
        const CourseManager& courseManager
    );

    // 敵の描画
    void DrawAll();

    // 敵の数を取得
    int GetCount() const;

    // 敵を取得
    Enemy& GetEnemy(int index);

    // 敵を全削除
    void Clear();
};

#endif