#include "Boss.h"
#include "DxLib.h"
#include <cmath>  

// 大ボスモデルの読み込み
bool Boss::LoadModel()
{
    // すでに読み込んでいる場合
    if (modelHandle != -1)
    {
        return true;
    }

    // アニメーション内蔵モデルを読み込む
    modelHandle = MV1LoadModel(
        "Resource/Boss/TitanModel_Animated.mv1"
    );

    if (modelHandle == -1)
    {
        return false;
    }

    // 内蔵アニメーションの数を取得
    int animNum = MV1GetAnimNum(modelHandle);

    printf("Animation Num: %d\n", animNum);

    if (animNum <= 0)
    {
        ReleaseModel();
        return false;
    }

    // 先頭のアニメーションを使用
    int animIndex = 0;

    // アニメーションを接続
    animAttachIndex = MV1AttachAnim(
        modelHandle,
        animIndex,
        -1,
        FALSE
    );

    if (animAttachIndex == -1)
    {
        printf("Animation Attach Failed\n");
        ReleaseModel();
        return false;
    }

    // 再生時間を初期化
    animTime = 0.0f;

    // 総再生時間を取得
    animTotalTime = MV1GetAttachAnimTotalTime(
        modelHandle,
        animAttachIndex
    );

    printf("Animation Total Time: %.2f\n", animTotalTime);

    MV1SetAttachAnimTime(
        modelHandle,
        animAttachIndex,
        0.0f
    );

    // すべて成功した場合だけtrueを返す


    return true;
}

void Boss::ReleaseModel()
{
    if (modelHandle != -1)
    {
        if (animAttachIndex != -1)
        {
            MV1DetachAnim(modelHandle, animAttachIndex);
            animAttachIndex = -1;
        }

        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }

    animTime = 0.0f;
    animTotalTime = 0.0f;
}

// 大ボスの更新
void Boss::Update(float deltaTime)
{
    if (!active ||
        modelHandle == -1 ||
        animAttachIndex == -1)
    {
        return;
    }

    // 再生時間を進める（30FPS想定）
    animTime += deltaTime * 30.0f;

    // アニメーションをループさせる
    if (animTotalTime > 0.0f)
    {
        if (animTime >= animTotalTime)
        {
            animTime = std::fmod(animTime, animTotalTime);
        }
    }

    // 再生時間をモデルに反映
    MV1SetAttachAnimTime(
        modelHandle,
        animAttachIndex,
        animTime
    );
}

// 大ボスの描画
void Boss::Draw()
{
    if (!active || modelHandle == -1)
    {
        return;
    }

    MV1SetScale(
        modelHandle,
        VGet(0.01f, 0.01f, 0.01f)
    );

    MV1SetPosition(
        modelHandle,
        VGet(x, y, z)
    );

    MV1DrawModel(modelHandle);

    DrawFormatString(
        10, 200,
        GetColor(255, 255, 255),
        "Anim: %.2f / %.2f",
        animTime,
        animTotalTime
    );
}

// ボスの出現
void Boss::Spawn(float startX, float startY, float startZ)
{
    x = startX;
    y = startY;
    z = startZ;

    hp = 50;
    active = true;
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

