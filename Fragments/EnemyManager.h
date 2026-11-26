#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include "Enemy.h"
#include "CourseManager.h"

class EnemyManager
{
private:
    //敵モデルのハンドル
    int modelHandle = -1;

    //現在管理している敵の配列
    std::vector<Enemy> enemies;

    //次の敵を出現させる位置
    float nextSpawnZ = 80.0f;

    //敵を生成する間隔
    static constexpr float SPAWN_INTERVAL = 50.0f;

public:

    // 敵モデルの読み込み
    bool LoadModel();

    // 敵モデルの解放
    void ReleaseModel();

    // 敵の初期生成
    void Initialize(const CourseManager& courseManager);

    // 敵の生成
    void SpawnEnemy(
        float courseZ,
        float laneOffset,
        float y,
        const CourseManager& courseManager
    );

    // 敵の自動生成を更新
    void Update(float playerCourseZ, const CourseManager& courseManager);


    // 敵の生成再開位置を設定
    void ResetSpawnPosition(float playerCourseZ);

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