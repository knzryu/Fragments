#include "Boss.h"
#include "DxLib.h"

// 大ボスのモデルと待機アニメーションを読み込む
bool Boss::LoadModel()
{
    // すでに読み込んでいる場合
    if (modelHandle != -1)
    {
        return true;
    }

    // 大ボス本体を読み込む
    modelHandle = MV1LoadModel("Resource/Boss/TitanModel.mv1");

    if (modelHandle == -1)
    {
        return false;
    }

    // テクスチャ情報を確認する
    int textureNum = MV1GetTextureNum(modelHandle);

    for (int i = 0; i < textureNum; i++)
    {
        const char* path = MV1GetTextureColorFilePath(modelHandle, i);

        if (path != nullptr)
        {
            printf("Texture %d : %s\n", i, path);
        }
    }

    // 待機アニメーションを読み込む
    idleAnimModelHandle = MV1LoadModel(
        "Resource/Boss/Animations/Titan_Idle.mv1"
    );

    if (idleAnimModelHandle == -1)
    {
        ReleaseModel();
        return false;
    }

    // 待機アニメーションを本体に接続
    idleAttachIndex = MV1AttachAnim(
        modelHandle,
        0,
        idleAnimModelHandle,
        TRUE  // FALSEからTRUEへ変更
    );

    if (idleAttachIndex == -1)
    {
        ReleaseModel();
        return false;
    }

    return true;
}

// 大ボスのモデルとアニメーションを解放
void Boss::ReleaseModel()
{
    // アニメーションの接続を解除
    if (modelHandle != -1 && idleAttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, idleAttachIndex);
        idleAttachIndex = -1;
    }

    // アニメーションモデルを解放
    if (idleAnimModelHandle != -1)
    {
        MV1DeleteModel(idleAnimModelHandle);
        idleAnimModelHandle = -1;
    }

    // 大ボス本体を解放
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}


// 大ボスの更新
void Boss::Update(float deltaTime)
{
    if (!active || modelHandle == -1 || idleAttachIndex == -1)
    {
        return;
    }

    // 待機アニメーションの再生時間を進める
    float currentTime = MV1GetAttachAnimTime(
        modelHandle,
        idleAttachIndex
    );


    currentTime += deltaTime * 30.0f;

    // アニメーションをループ再生
    float totalTime = MV1GetAttachAnimTotalTime(
        modelHandle,
        idleAttachIndex
    );

    if (totalTime > 0.0f)
    {
        while (currentTime >= totalTime)
        {
            currentTime -= totalTime;
        }
    }

    MV1SetAttachAnimTime(
        modelHandle,
        idleAttachIndex,
        currentTime
    );

  
}

// 大ボスの描画
void Boss::Draw()
{
    if (!active || modelHandle == -1)
    {
        return;
    }

    // モデルサイズ設定
    MV1SetScale(modelHandle, VGet(0.01f, 0.01f, 0.01f));

    // モデルの位置を設定
    MV1SetPosition(modelHandle, VGet(x, y, z));

    // モデルを描画
    MV1DrawModel(modelHandle);

    // アニメーション時間を取得
    float currentTime = MV1GetAttachAnimTime(
        modelHandle,
        idleAttachIndex
    );

    float totalTime = MV1GetAttachAnimTotalTime(
        modelHandle,
        idleAttachIndex
    );

    // アニメーション時間を画面に表示
    DrawFormatString(
        10, 200,
        GetColor(255, 255, 255),
        "Anim Time: %.2f / %.2f",
        currentTime,
        totalTime
    );
}

//ボスの出現
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