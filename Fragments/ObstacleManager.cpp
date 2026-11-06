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
void ObstacleManager::SpawnObstacle(float spawnZ, float roadCenterX)   
{
    // 生成するレーンをランダムに決める
    //GetRandは０、１、２のいずれかをランダムに返す
   // 障害物の幅
    const float obstacleWidth = 2.0f;

    // コースの端から余裕を確保した生成範囲
    const float minOffset = -8.0f;
    const float maxOffset = 8.0f;

    // ランダムな位置を決定する
    float randomOffset =
        minOffset + (GetRand(1600) / 100.0f);

    // コース中心を基準に障害物の位置を決める
    float obstacleCenterX = roadCenterX + randomOffset;

    float minX = obstacleCenterX - obstacleWidth / 2.0f;
    float maxX = obstacleCenterX + obstacleWidth / 2.0f;

    // 障害物を追加する
    obstacles.push_back(
        {
            minX, maxX,
            0.0f, 3.0f,
            spawnZ, spawnZ + 2.0f
        }
    );
 
}
// 障害物を初期状態に戻す
void ObstacleManager::Reset()
{
    // 障害物をすべて削除
    obstacles.clear();
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

// 障害物をすべて描画する
void ObstacleManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(obstacles.size()); i++)
    {
		// 一番目の障害物から順番に描画する
        obstacles[i].Draw();
    }
}