#include "Boss.h"

//中ボスの出現
void Boss::Spawn(float startX, float startY, float startZ)
{
    x = startX;
    y = startY;
    z = startZ;

    hp = 50;
    active = true;
}

// ダメージを受ける
void Boss::TakeDamage(int damage)
{
    if (!active || damage <= 0)
    {
        return;
    }

    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}

// 撃破判定
bool Boss::IsDead() const
{
    return hp <= 0;
}

// 出現状態を取得
bool Boss::IsActive() const
{
    return active;
}

// HPを取得
int Boss::GetHP() const
{
    return hp;
}