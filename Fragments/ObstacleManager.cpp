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

// 障害物生成関数
void ObstacleManager::SpawnObstacle(
    float courseZ,
    const CourseManager& courseManager)
{
    // 障害物の横方向の配置範囲
    const float minOffset = -3.5f;
    const float maxOffset = 3.5f;

    // ランダムな横方向の位置を決定
    float randomOffset =
        minOffset + GetRand(700) / 100.0f;

    // 障害物を生成
    Obstacle obstacle;

    // コース上の位置を設定
    obstacle.courseZ = courseZ;
    obstacle.laneOffset = randomOffset;

    // ワールド座標を更新
    obstacle.UpdatePosition(courseManager);

    // 管理リストに追加
    obstacles.push_back(obstacle);
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
        if (obstacles[i].courseZ < playerZ - 5.0f)
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