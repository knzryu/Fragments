
#include "LaserAttack.h"

#include <cmath>

void LaserAttack::SetDirection(
    VECTOR origin,
    CoursePoint forward,
    CoursePoint right)
{
    // 始点を設定
    startPosition = origin;

    constexpr float PI = 3.14159265f;

    // 角度をラジアンに変換
    float angle = LASER_ANGLE * PI / 180.0f;

    // 左・中央・右の角度
    float angles[3] =
    {
        -angle,
        0.0f,
        angle
    };

    for (int i = 0; i < 3; i++)
    {
        float cosAngle = std::cos(angles[i]);
        float sinAngle = std::sin(angles[i]);

        // コースの進行方向を反転してプレイヤー側へ向ける
        float dirX =
            -forward.x * cosAngle +
            right.x * sinAngle;

        float dirZ =
            -forward.z * cosAngle +
            right.z * sinAngle;

        // 終点を計算
        endPositions[i] = VGet(
            origin.x + dirX * LASER_LENGTH,
            origin.y,
            origin.z + dirZ * LASER_LENGTH
        );
    }
}

void LaserAttack::DrawWarning() const
{
    for (int i = 0; i < 3; i++)
    {
        DrawLine3D(
            startPosition,
            endPositions[i],
            GetColor(255, 0, 0)
        );
    }
}