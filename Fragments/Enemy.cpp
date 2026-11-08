#include "DxLib.h"
#include "Enemy.h"

//敵の初期化
void Enemy::Initialize(float startX, float startY, float startZ)
{
    x = startX;
    y = startY;
    z = startZ;
    hp = 3;
}

// コース情報から敵のワールド座標を更新する
void Enemy::UpdatePosition(const CourseManager& courseManager)
{
    // コース上の位置から道路の中心を取得
    CoursePoint center = courseManager.GetPosition(courseZ);

    // コース上の位置から道路の右方向を取得
    CoursePoint right = courseManager.GetRight(courseZ);

    // 道路の中心から横方向にずらした敵の座標
    x = center.x + right.x * laneOffset;
    z = center.z + right.z * laneOffset;
}


void Enemy::Draw()
{
    DrawCube3D(
        VGet(x - 0.5f, y, z - 0.5f),
        VGet(x + 0.5f, y + 2.0f, z + 0.5f),
        GetColor(255, 80, 80),
        GetColor(255, 80, 80),
        TRUE
    );
}
//TakeDamageの処理中身
void Enemy::TakeDamage(int damage)
{
    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}