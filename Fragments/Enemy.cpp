#include "DxLib.h"
#include "Enemy.h"

//敵の初期化
void Enemy::Initialize(float startX, float startY, float startZ)
{
    x = startX;
    y = startY;
    z = startZ;
    hp = 1;
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


void Enemy::Draw(int modelHandle)
{
    // モデルサイズ（仮の調整値）
    MV1SetScale(modelHandle, VGet(0.02f, 0.02f, 0.02f));

    //敵のモデル座標
    MV1SetPosition(modelHandle,VGet(x, y + 2.0f, z));

    //モデルを描画
    MV1DrawModel(modelHandle);
}

//TakeDamageの処理中身
void Enemy::TakeDamage(int damage)
{
    //0以下のダメージは受け付けない
    if (hp < 0)
    {
        hp = 0;
    }

    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}

//HPを取得
int Enemy::GetHP() const
{
    return hp;
}

//敵が倒されているか確認する
bool Enemy::IsDead() const
{
    return hp <= 0;
}