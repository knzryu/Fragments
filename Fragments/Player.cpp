#include "Player.h"
#include "DxLib.h"
#include <math.h>

//プレイヤーを3Dの立方体として描画
void Player::Initialize()
{
    //初期化処理
    x = 0.0f;
    z = 0.0f;
    forwardSpeed = 10.0f;
    sideSpeed = 5.0f;

    // 3Dモデルを読み込む
    modelHandle = MV1LoadModel("Models/PlayerModel.mv1");

    if (modelHandle == -1)
    {
        printfDx("PlayerModel.mv1の読み込みに失敗しました");
        return;
    }

    // モデルが持っているアニメーション数を確認
    int animNum = MV1GetAnimNum(modelHandle);

    if (animNum > 0)
    {
        // 先頭のアニメーションをモデルに取り付ける
        attachIndex = MV1AttachAnim(modelHandle, 0, -1, FALSE);
    }
    else
    {
        printfDx("モデルにアニメーションがありません");
    }

    // アニメーションの総再生時間を取得
    animTotalTime = MV1GetAttachAnimTotalTime(
        modelHandle,
        attachIndex
    );

    // 再生時間を初期化
    animTime = 0.0f;

    // 最初の姿勢をモデルに反映
    MV1SetAttachAnimTime(modelHandle, attachIndex, animTime);
}

void Player::Reset()
{
    // プレイヤーの位置を初期化
    x = 0.0f;
    z = 0.0f;

    // 移動速度を初期化
    forwardSpeed = 10.0f;
    sideSpeed = 5.0f;

    // アニメーションを最初から再生
    animTime = 0.0f;

    if (modelHandle != -1 && attachIndex != -1)
    {
        MV1SetAttachAnimTime(
            modelHandle,
            attachIndex,
            animTime
        );
    }
}
//描画
void Player::Draw()
{
    // モデルが正常に読み込まれている場合
    if (modelHandle != -1)
    {
        // コースの傾きを計算
        float slope = 20.0f * 0.01f * cosf(z * 0.01f);

        // 進行方向の角度を計算
        float angle = atan2f(slope, 1.0f);

        // モデルを進行方向に回転
        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angle, 0.0f)
        );

        // モデルを10分の1に縮小
        MV1SetScale(modelHandle, VGet(0.015f, 0.015f, 0.015f));

        // プレイヤーの座標にモデルを配置
        MV1SetPosition(modelHandle, VGet(x, 0.0f, z));

        // 3Dモデルを描画
        MV1DrawModel(modelHandle);
    }
}

//プレイヤーを前進させる関数の定義
void Player::MoveForward(float deltaTime)
{
    // コースのカーブの傾きを計算
    float slope = 20.0f * 0.01f * cosf(z * 0.01f);

    // 前進速度からZ方向の移動量を計算
    float moveZ = forwardSpeed * deltaTime / sqrtf(1.0f + slope * slope);

    // カーブに沿ってX方向にも移動
    float moveX = slope * moveZ;

    x += moveX;
    z += moveZ;
}

// 左移動
void Player::MoveLeft(float deltaTime)
{
    float slope = 20.0f * 0.01f * cosf(z * 0.01f);

    float moveX = sideSpeed * deltaTime / sqrtf(1.0f + slope * slope);
    float moveZ = -slope * moveX;

    x -= moveX;
    z += moveZ;
}

// 右移動
void Player::MoveRight(float deltaTime)
{
    float slope = 20.0f * 0.01f * cosf(z * 0.01f);

    float moveX = sideSpeed * deltaTime / sqrtf(1.0f + slope * slope);
    float moveZ = -slope * moveX;

    x += moveX;
    z += moveZ;
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

void Player::Finalize()
{
    // モデルが読み込まれている場合だけ解放する
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}

void Player::UpdateAnimation(float deltaTime)
{
    // アニメーションが設定されていない場合は終了
    if (attachIndex == -1 || animTotalTime <= 0.0f)
    {
        return;
    }

    // 再生時間を進める
    animTime += deltaTime * 1000.0f * animSpeed;

    // 総再生時間を超えたらループさせる
    if (animTime >= animTotalTime)
    {
        animTime -= animTotalTime;
    }

    // 新しい再生時間をモデルに反映
    MV1SetAttachAnimTime(
        modelHandle,
        attachIndex,
        animTime
    );
}