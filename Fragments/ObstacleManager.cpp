#include "ObstacleManager.h"
#include"DxLib.h"

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