#include "DxLib.h"
#include "Bullet.h"

//弾移動処理
void Bullet::Update(float deltaTime)
{
    //速度×経過時間でZ座標更新する
    z += speed * deltaTime;
}

//描画処理
void Bullet::Draw()
{
    //弾が無効なら、ここで関数を終了する
    if (!active)
    {
        return;
    }

    //立方体の弾
    DrawCube3D(
        VGet(x - 0.15f, y - 0.15f, z - 0.15f),
        VGet(x + 0.15f, y + 0.15f, z + 0.15f),
        GetColor(255, 255, 0),
        GetColor(255, 255, 0),
        TRUE
    );
}