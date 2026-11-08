#include "DxLib.h"
#include "Obstacle.h"



//障害物の描画関数
void Obstacle::Draw()
{
    //SetUseLighting(FALSE);

    DrawCube3D(
        VGet(minX, minY, minZ),
        VGet(maxX, maxY, maxZ),
        GetColor(0, 255, 0),
        GetColor(0, 255, 0),
        TRUE
    );
}

// コース情報から障害物のワールド座標を更新する
void Obstacle::UpdatePosition(const CourseManager& courseManager)
{
    // コース上の位置から道路の中心を取得
    CoursePoint center = courseManager.GetPosition(courseZ);

    // コース上の位置から道路の右方向を取得
    CoursePoint right = courseManager.GetRight(courseZ);

    // 道路の中心から横方向にずらした障害物の中心座標
    float centerX = center.x + right.x * laneOffset;
    float centerZ = center.z + right.z * laneOffset;

    // X方向の境界値
    minX = centerX - width / 2.0f;
    maxX = centerX + width / 2.0f;

    // Y方向の境界値
    minY = centerY - height / 2.0f;
    maxY = centerY + height / 2.0f;

    // Z方向の境界値
    minZ = centerZ - depth / 2.0f;
    maxZ = centerZ + depth / 2.0f;
}