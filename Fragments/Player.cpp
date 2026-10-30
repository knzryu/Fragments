#include "Player.h"
#include "DxLib.h"

//プレイヤーを3Dの立方体として描画
void Player::Initialize()
{
    //初期化処理
    x = 0.0f;
    z = 0.0f;
    forwardSpeed = 10.0f;
    sideSpeed = 10.0f;
}

void Player::Reset()
{
    Initialize();
}

//描画
void Player::Draw()
{
    //プレイヤーを3Dの立方体として描画
    DrawCube3D(
        VGet(x - 0.5f, 0.0f, z - 0.5f), //左下奥の座標
        VGet(x + 0.5f, 2.0f, z + 0.5f), //右上手前の座標
        GetColor(0, 200, 255),            //赤色
		GetColor(0, 200, 255),            //赤色
        TRUE                            //塗りつぶしあり
    );
}

//プレイヤーを前進させる関数の定義
void Player::MoveForward(float deltaTime)
{
    //前進速度×経過時間をZ座標に加算する
    z += forwardSpeed * deltaTime;
}
//左移動
void Player::MoveLeft(float deltaTime)
{
    //左方向に移動するため、X座標から移動量を引く
    x -= sideSpeed * deltaTime;
}
//右移動
void Player::MoveRight(float deltaTime)
{
    //右方向に移動するため、X座標に移動量をプラスする
    x += sideSpeed * deltaTime;
}
// 前進速度の設定
void Player::SetForwardSpeed(float newSpeed)
{
    forwardSpeed = newSpeed;
}

void Player::SetSideSpeed(float newSpeed)
{
    sideSpeed = newSpeed;
}

//Z座標の取得
float Player::GetZ() const
{
    return z;
}
float Player::GetX() const
{
    return x;
}
void Player::SetX(float newX)
{
    x = newX;
}

void Player::SetZ(float newZ)
{
    z = newZ;
}


// 攻撃力の取得
int Player::GetAttackPower() const
{
    return attackPower;
}

// 攻撃力の設定
void Player::SetAttackPower(int newPower)
{
    attackPower = newPower;
}

// 弾速の取得
float Player::GetBulletSpeed() const
{
    return bulletSpeed;
}

// 弾速の設定
void Player::SetBulletSpeed(float newSpeed)
{
    bulletSpeed = newSpeed;
}