#include "DxLib.h"
#include "Enemy.h"

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
//TakeDamage‚Ìˆ—’†g
void Enemy::TakeDamage(int damage)
{
    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}