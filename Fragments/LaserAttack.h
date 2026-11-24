#pragma once

#include "DxLib.h"
#include "CourseManager.h"

//ボス攻撃
class LaserAttack
{
private:

    // レーザーの始点
    VECTOR startPosition = VGet(0.0f, 0.0f, 0.0f);

    // 3本のレーザーの終点
    VECTOR endPositions[3]{};

    // レーザーの長さ
    static constexpr float LASER_LENGTH = 80.0f;

    // 左右に広げる角度
    static constexpr float LASER_ANGLE = 20.0f;

public:

    // レーザーの方向を計算
    void SetDirection(
        VECTOR origin,
        CoursePoint forward,
        CoursePoint right
    );

    // 予兆線を描画
    void DrawWarning() const;
};