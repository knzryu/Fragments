#include "ObstacleManager.h"
#include"DxLib.h"


// 管理している障害物の個数を取得する
int ObstacleManager::GetCount() const
{
    return static_cast<int>(obstacles.size());
}

// 指定した番号の障害物を取得する
Obstacle& ObstacleManager::GetObstacle(int index)
{
    return obstacles[index];
}
//障害物生成関数
void ObstacleManager::SpawnObstacle(float spawnZ)   
{
    // 生成するレーンをランダムに決める
    //GetRandは０、１、２のいずれかをランダムに返す
    int lane = GetRand(2);

    float minX;
    float maxX;

    if (lane == 0)
    {
        minX = -5.0f;
        maxX = -3.0f;
    }
    else if (lane == 1)
    {
        minX = -1.0f;
        maxX = 1.0f;
    }
    else
    {
        minX = 3.0f;
        maxX = 5.0f;
    }

    // obstaclesに障害物を一個追加する
    obstacles.push_back(
        {
            minX, maxX,           //X座標の範囲
            0.0f, 3.0f,           //Y座標の範囲
            spawnZ, spawnZ + 2.0f //Z座標の範囲
        }
    );
}
// 障害物を初期状態に戻す
void ObstacleManager::Reset()
{
    // 障害物をすべて削除
    obstacles.clear();

    // 初期障害物を配置
    obstacles.push_back(
        { -1.0f, 1.0f, 0.0f, 3.0f, 20.0f, 22.0f }
    );

    obstacles.push_back(
        { 3.0f, 5.0f, 0.0f, 3.0f, 30.0f, 32.0f }
    );

    obstacles.push_back(
        { -5.0f, -3.0f, 0.0f, 3.0f, 40.0f, 42.0f }
    );

    // 初期障害物のレーンをランダムに変更
    for (int i = 0;
        i < static_cast<int>(obstacles.size());
        i++)
    {
        int lane = GetRand(2);

        if (lane == 0)
        {
            obstacles[i].minX = -5.0f;
            obstacles[i].maxX = -3.0f;
        }
        else if (lane == 1)
        {
            obstacles[i].minX = -1.0f;
            obstacles[i].maxX = 1.0f;
        }
        else
        {
            obstacles[i].minX = 3.0f;
            obstacles[i].maxX = 5.0f;
        }
    }
}
// プレイヤーの後方に離れた障害物を削除する
void ObstacleManager::RemovePassedObstacles(float playerZ)
{
    // 障害物の個数をint型に変換し、最後のインデックスを求める
    for (int i = static_cast<int>(obstacles.size()) - 1;
        i >= 0;
        i--)
    {
        // プレイヤーの後方に離れた障害物を削除する条件式
        if (obstacles[i].maxZ < playerZ - 5.0f)
        {
            obstacles.erase(obstacles.begin() + i);
        }
    }
}
void ObstacleManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(obstacles.size()); i++)
    {
		// 一番目の障害物から順番に描画する
        obstacles[i].Draw();
    }
}