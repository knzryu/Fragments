#include "Player.h"
#include "DxLib.h"

//プレイヤーを3Dの立方体として描画
void Player::Draw()
{
    DrawCube3D(
        //立方体の最小座標（左下奥側）
        VGet(x - 0.5f, 0.0f, z - 0.5f),
        //立方体の最大座標（右上手前側）
        VGet(x + 0.5f, 2.0f, z + 0.5f),
        //表面の色
        GetColor(0, 200, 255),
        //裏面の色
        GetColor(0, 200, 255),
        //塗りつぶして描画
        TRUE
    );
}

//プレイヤーを前進させる関数の定義
void Player::MoveForward(float deltaTime)
{
    //前進速度×経過時間をZ座標に加算する
    z += forwardSpeed * deltaTime;
}

void Player::MoveLeft(float deltaTime)
{
    //左方向に移動するため、X座標から移動量を引く
    x -= sideSpeed * deltaTime;
}

void Player::MoveRight(float deltaTime)
{
    //右方向に移動するため、X座標に移動量をプラスする
    x += sideSpeed * deltaTime;
}

//プレイヤーの前進速度を設定する関数の定義
void Player::SetForwardSpeed(float newSpeed)
{
    //引数で受け取った速度をメンバ変数に保存する
    forwardSpeed = newSpeed;
}

void Player::SetSideSpeed(float newSpeed)
{
    sideSpeed = newSpeed;
}
