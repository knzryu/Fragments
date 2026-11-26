#include "Boss.h"
#include "DxLib.h"
#include "CourseManager.h"
#include <cmath>  

// 大ボスモデルの読み込み
bool Boss::LoadModel()
{
    
    // レーザー用テクスチャを読み込む
    if (laserTextureHandle == -1)
    {
        laserTextureHandle =
            LoadGraph("Resource/Effect/LaserBeamTexture.png");

        if (laserTextureHandle == -1)
        {
            printf("Laser Texture Load Failed\n");
            return false;
        }

        laserAttack.SetTextureHandle(laserTextureHandle);
    }

    //----------------------------------------
    //         bossモデル

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
    // レーザー用テクスチャを解放
    if (laserTextureHandle != -1)
    {
        DeleteGraph(laserTextureHandle);
        laserTextureHandle = -1;

        laserAttack.SetTextureHandle(-1);
    }
    //ボス用モデル解放
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
void Boss::Update(float deltaTime, const CourseManager& courseManager)
{
    // ボスが出現していない場合は更新しない
    if (!active || IsDead())
    {
        return;
    }

    // 再生時間を進める（30FPS想定）
    actionTimer += deltaTime;

    switch (action)
    {
    case BossAction::Idle:

        if (actionTimer >= 1.5f)
        {
            // 次のレーンを決める
            currentLane = (currentLane + 1) % 3;

            // レーン番号から目標オフセットを決める
            targetLaneOffset = (currentLane - 1) * 4.0f;

            // 1秒の予兆時間で移動が完了する速度を計算
            float moveDistance =
                std::fabs(targetLaneOffset - laneOffset);

            laneMoveSpeed = moveDistance / 1.0f;

            action = BossAction::Warning;
            actionTimer = 0.0f;
        }
        break;

    case BossAction::Warning:

        // 攻撃予兆中にレーン移動
        UpdateLane(deltaTime);

        if (actionTimer >= 1.0f)
        {
            action = BossAction::Charging;
            actionTimer = 0.0f;
        }
        break;

    case BossAction::Charging:

        // チャージ完了で攻撃開始
        if (actionTimer >= CHARGE_DURATION)
        {
            action = BossAction::Attacking;
            actionTimer = 0.0f;
        }
        break;

    case BossAction::Attacking:

        // 0.5秒攻撃状態を維持したら硬直へ
        if (actionTimer >= 0.5f)
        {
            action = BossAction::Recovery;
            actionTimer = 0.0f;
        }
        break;

    case BossAction::Recovery:

        // 1.2秒硬直したら待機へ
        if (actionTimer >= 1.2f)
        {
            action = BossAction::Idle;
            actionTimer = 0.0f;
        }
        break;

    case BossAction::Dead:
        return;
    }

    // コース中心を取得
    CoursePoint center =
        courseManager.GetPosition(bossCourseZ);

    // コースの右方向を取得
    CoursePoint right =
        courseManager.GetRight(bossCourseZ);

    // ワールド座標を更新
    x = center.x + right.x * laneOffset;
    z = center.z + right.z * laneOffset;



    // レーザーの予兆中だけ方向を更新する
    if (action == BossAction::Warning)
    {
        // コースの進行方向を取得
        CoursePoint forward =
            courseManager.GetDirection(bossCourseZ);

        // コースの右方向を取得
        CoursePoint right =
            courseManager.GetRight(bossCourseZ);

        // レーザーの始点をボスの位置に設定
        VECTOR origin = VGet(
            x,
            y + 2.0f,
            z
        );

        // 3本のレーザーの方向を計算
        laserAttack.SetDirection(
            origin,
            forward,
            right
        );
    }

    // --------------------------------
    // ② アニメーションの更新
    // --------------------------------

    // アニメーションがない場合は終了
    if (modelHandle == -1 ||
        animAttachIndex == -1 ||
        animTotalTime <= 0.0f)
    {
        return;
    }

    // 再生時間を進める
    animTime += deltaTime * 30.0f;

    // アニメーションをループさせる
    if (animTime >= animTotalTime)
    {
        animTime = std::fmod(animTime, animTotalTime);
    }

    // 再生時間をモデルに反映
    MV1SetAttachAnimTime(
        modelHandle,
        animAttachIndex,
        animTime
    );
}

// ボスの描画
void Boss::Draw()
{
    if (!active)
    {
        return;
    }

    // ボスモデルの描画
    if (modelHandle != -1)
    {
        MV1SetScale(
            modelHandle,
            VGet(0.01f, 0.01f, 0.01f)
        );

        MV1SetPosition(
            modelHandle,
            VGet(x, y, z)
        );

        MV1DrawModel(modelHandle);
    }

    // 行動状態によってレーザーの見た目を切り替える
    if (action == BossAction::Warning)
    {
        // 予兆中：オレンジ色の細い線
        laserAttack.DrawWarning();
    }
    else if (action == BossAction::Charging)
    {
        float progress = actionTimer / CHARGE_DURATION;

        laserAttack.DrawCharge(progress);
    }
    else if (action == BossAction::Attacking)
    {
        // 攻撃中：赤色の太いレーザー
        laserAttack.DrawAttack();
    }
    else if (action == BossAction::Recovery)
    {
        float progress = actionTimer / 1.2f;

        laserAttack.DrawEnd(progress);
    }
}

// ボスの出現
void Boss::Spawn(float startCourseZ,float startLaneOffset,const CourseManager& courseManager)
{
    bossCourseZ = startCourseZ;

    laneOffset = startLaneOffset;
    targetLaneOffset = startLaneOffset;

    currentLane = 1;

    hp = INITIAL_HP;
    active = true;

    action = BossAction::Idle;
    actionTimer = 0.0f;

    // 初期位置を計算
    CoursePoint center =
        courseManager.GetPosition(bossCourseZ);

    CoursePoint right =
        courseManager.GetRight(bossCourseZ);

    x = center.x + right.x * laneOffset;
    z = center.z + right.z * laneOffset;
    y = 0.0f;
}


// 弾がボスに命中したか判定
bool Boss::IsHit(
    float bulletPreviousCourseZ,
    float bulletCourseZ,
    float bulletLaneOffset,
    float bulletSize) const
{
    // ボスが出現していない、または撃破済みなら命中しない
    if (!active || IsDead())
    {
        return false;
    }

    // 当たり判定の半分の大きさ
    constexpr float HIT_HALF_WIDTH = 1.0f;
    constexpr float HIT_HALF_DEPTH = 1.0f;
    

    // 横方向の当たり判定
    float distanceX = std::fabs(bulletLaneOffset - laneOffset);

    bool hitX = distanceX <= HIT_HALF_WIDTH + bulletSize;

    // 奥行き方向の当たり判定（弾の移動区間を調べる）
    bool hitZ =
        bulletPreviousCourseZ <= bossCourseZ + HIT_HALF_DEPTH + bulletSize &&
        bulletCourseZ >= bossCourseZ - HIT_HALF_DEPTH - bulletSize;

    return hitX && hitZ;
}

// レーザーがプレイヤーに命中したか判定
bool Boss::IsLaserHit(
    VECTOR playerMin,
    VECTOR playerMax) const
{
    // 攻撃中以外は命中しない
    if (!active || IsDead() ||
        action != BossAction::Attacking)
    {
        return false;
    }

    return laserAttack.IsHit(
        playerMin,
        playerMax
    );
}


// ダメージを受ける
void Boss::TakeDamage(int damage)
{
    if (!active || IsDead() || damage <= 0)
    {
        return;
    }

    hp -= damage;

    if (hp <= 0)
    {
        hp = 0;
        action = BossAction::Dead;
        actionTimer = 0.0f;
    }
}


// レーンを滑らかに移動させる
void Boss::UpdateLane(float deltaTime)
{
    float difference = targetLaneOffset - laneOffset;

    float moveDistance = laneMoveSpeed * deltaTime;

    // 目標までの距離が移動量以下なら到着
    if (std::fabs(difference) <= moveDistance)
    {
        laneOffset = targetLaneOffset;
    }
    else
    {
        // 目標方向へ移動
        if (difference > 0.0f)
        {
            laneOffset += moveDistance;
        }
        else
        {
            laneOffset -= moveDistance;
        }
    }
}


void Boss::Reset()
{
    // HPを初期値に戻す
    hp = INITIAL_HP;

    // ボスを未出現状態にする
    active = false;

    // 座標を初期化
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

    // 行動状態を初期化
    action = BossAction::Idle;
    actionTimer = 0.0f;

    // アニメーション時間を初期化
    animTime = 0.0f;

    // 読み込み済みモデルがある場合は姿勢も初期化
    if (modelHandle != -1 && animAttachIndex != -1)
    {
        MV1SetAttachAnimTime(
            modelHandle,
            animAttachIndex,
            animTime
        );
    }
}

BossAction Boss::GetAction() const
{
    return action;
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

// 最大HPを取得
int Boss::GetMaxHP() const
{
    return INITIAL_HP;
}