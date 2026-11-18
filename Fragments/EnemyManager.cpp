#include "DxLib.h"
#include "EnemyManager.h"
#include <cstdio>

// 敵モデルの読み込み
bool EnemyManager::LoadModel()
{
    // すでに読み込まれている場合は成功扱い
    if (modelHandle != -1)
    {
        return true;
    }

    modelHandle = MV1LoadModel(
        "Resource/Enemy/DroneModel.mv1"
    );

    // 読み込みに失敗した場合
    if (modelHandle == -1)
    {
        return false;
    }

    

    return true;
}

// 敵モデルの解放
void EnemyManager::ReleaseModel()
{
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}

// 敵の初期生成
void EnemyManager::Initialize(const CourseManager& courseManager)
{
    Clear();

    //次の敵の出現位置を初期化
    nextSpawnZ = 60.0f;

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

   

    // ワールド座標を計算
    enemy.UpdatePosition(courseManager);

    // 管理配列に追加
    enemies.push_back(enemy);

   
}


// 敵の自動生成を更新
void EnemyManager::Update(
    float playerCourseZ,
    const CourseManager& courseManager)
{
    // プレイヤーの50m先まで敵を生成する
    while (nextSpawnZ < playerCourseZ + 50.0f)
    {
        // -1、0、1 のいずれかをランダムに取得
        int laneIndex = GetRand(2) - 1;

        // レーン位置を決定
        float laneOffset = laneIndex * 3.0f;

        // 敵を生成
        SpawnEnemy(
            nextSpawnZ,
            laneOffset,
            0.0f,
            courseManager
        );

        // 次の出現位置へ進める
        nextSpawnZ += SPAWN_INTERVAL;
    }
}


// 敵の描画 
void EnemyManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (!enemies[i].IsDead())
        {
            enemies[i].Draw(modelHandle);
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