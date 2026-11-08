#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "CourseManager.h"

// 障害物クラス
class Obstacle
{
public:
    // コース上の位置
    float courseZ = 0.0f;
    float laneOffset = 0.0f;

    // 障害物の大きさ
    float width = 2.0f;
    float height = 3.0f;
    float depth = 2.0f;

    // 障害物の中心の高さ
    float centerY = 1.5f;

    // ワールド座標の境界値（描画・当たり判定用）
    float minX = 0.0f;
    float maxX = 0.0f;
    float minY = 0.0f;
    float maxY = 0.0f;
    float minZ = 0.0f;
    float maxZ = 0.0f;

    // コース情報からワールド座標を更新
    void UpdatePosition(const CourseManager& courseManager);

    // 描画
    void Draw();
};

#endif