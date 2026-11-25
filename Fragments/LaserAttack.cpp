
#include "LaserAttack.h"

#include <cmath>
#include <algorithm>



// 線分とAABBの交差判定
namespace
{
    bool IsSegmentIntersectAABB(
        VECTOR start,
        VECTOR end,
        VECTOR boxMin,
        VECTOR boxMax)
    {
        float tMin = 0.0f;
        float tMax = 1.0f;

        float startPos[3] = {
            start.x, start.y, start.z
        };

        float direction[3] = {
            end.x - start.x,
            end.y - start.y,
            end.z - start.z
        };

        float minPos[3] = {
            boxMin.x, boxMin.y, boxMin.z
        };

        float maxPos[3] = {
            boxMax.x, boxMax.y, boxMax.z
        };

        for (int i = 0; i < 3; i++)
        {
            if (std::fabs(direction[i]) < 0.000001f)
            {
                if (startPos[i] < minPos[i] ||
                    startPos[i] > maxPos[i])
                {
                    return false;
                }

                continue;
            }

            float t1 =
                (minPos[i] - startPos[i]) / direction[i];

            float t2 =
                (maxPos[i] - startPos[i]) / direction[i];

            if (t1 > t2)
            {
                std::swap(t1, t2);
            }

            tMin = (std::max)(tMin, t1);
            tMax = (std::min)(tMax, t2);

            if (tMin > tMax)
            {
                return false;
            }
        }

        return true;
    }
}

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

//予兆線
void LaserAttack::DrawWarning() const
{
    for (int i = 0; i < 3; i++)
    {
        DrawLine3D(
            startPosition,
            endPositions[i],
            GetColor(255, 180, 0)
        );
    }
}

// 攻撃中のレーザーを描画
void LaserAttack::DrawAttack() const
{
    if (textureHandle == -1)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        DrawLaserBeam(
            startPosition,
            endPositions[i],
            LASER_VISUAL_WIDTH,
            textureHandle
        );
    }
}

//レーザー消失演出
void LaserAttack::DrawEnd(float progress) const
{
    
    //追加予定
}


// チャージ中のエネルギー球を描画
void LaserAttack::DrawCharge(float progress) const
{
    // 進行度を0.0～1.0に制限
    if (progress < 0.0f)
    {
        progress = 0.0f;
    }
    else if (progress > 1.0f)
    {
        progress = 1.0f;
    }

    // 時間経過で球を大きくする
    float radius = 0.3f + progress * 0.9f;

    /*外側の赤いエネルギー球
    DrawSphere3D(
        startPosition,
        radius,
        16,
        GetColor(255, 30, 30),
        GetColor(255, 100, 100),
        TRUE
    );

    // 中心の白い光
    DrawSphere3D(
        startPosition,
        radius * 0.35f,
        12,
        GetColor(255, 255, 255),
        GetColor(255, 255, 255),
        TRUE
    );*/
}


bool LaserAttack::IsHit(
    VECTOR playerMin,
    VECTOR playerMax) const
{
    // レーザーの半径分だけ当たり判定を拡張
    VECTOR expandedMin = VGet(
        playerMin.x - LASER_RADIUS,
        playerMin.y - LASER_RADIUS,
        playerMin.z - LASER_RADIUS
    );

    VECTOR expandedMax = VGet(
        playerMax.x + LASER_RADIUS,
        playerMax.y + LASER_RADIUS,
        playerMax.z + LASER_RADIUS
    );

    for (int i = 0; i < 3; i++)
    {
        if (IsSegmentIntersectAABB(
            startPosition,
            endPositions[i],
            expandedMin,
            expandedMax))
        {
            return true;
        }
    }

    return false;
}

void LaserAttack::SetTextureHandle(int handle)
{
    textureHandle = handle;
}