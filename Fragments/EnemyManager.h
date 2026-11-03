#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include "Enemy.h"

class EnemyManager
{
private:
    // 敵を管理する配列
    std::vector<Enemy> enemies;

public:
    // 敵の初期生成
    void Initialize();

    // 敵の生成
    void SpawnEnemy(float x, float y, float z);

    // 敵の描画
    void DrawAll();

	// 敵の数を取得
    int GetCount() const;

    //敵のを取得
    Enemy& GetEnemy(int index);

    // 敵を全削除
    void Clear();
};

#endif