#include "PlayerController.h"
#include "DxLib.h"

// プレイヤーの入力と移動を更新
void PlayerController::Update(Player& player, float deltaTime)
{
    // キー入力状態を取得
    bool aKey = CheckHitKey(KEY_INPUT_A);
    bool dKey = CheckHitKey(KEY_INPUT_D);
    bool spaceKey = CheckHitKey(KEY_INPUT_SPACE);


    // キーが押された瞬間を検出
    bool aKeyDown = aKey && !previousAKey;
    bool dKeyDown = dKey && !previousDKey;
    bool spaceKeyDown = spaceKey && !previousSpaceKey;

    // 最後に押した左右キーを記録
    if (aKeyDown)
    {
        lastMoveDirection = -1;
    }

    if (dKeyDown)
    {
        lastMoveDirection = 1;
    }

    // クールタイムを更新
    if (dashCooldownTimer > 0.0f)
    {
        dashCooldownTimer -= deltaTime;

        if (dashCooldownTimer < 0.0f)
        {
            dashCooldownTimer = 0.0f;
        }
    }

    // ダッシュ開始判定
    if (spaceKeyDown &&
        !isDashing &&
        dashCooldownTimer <= 0.0f &&
        lastMoveDirection != 0)
    {
        isDashing = true;
        dashTimer = 0.0f;
        dashMovedDistance = 0.0f;

        // 最後に押した方向を保存
        dashDirection = lastMoveDirection;

        // クールタイム開始
        dashCooldownTimer = DASH_COOLDOWN;
    }

    // ダッシュ中の処理
    if (isDashing)
    {
        // 経過時間を更新
        dashTimer += deltaTime;

        // 進行割合を0～1に制限
        float progress = dashTimer / DASH_DURATION;

        if (progress > 1.0f)
        {
            progress = 1.0f;
        }

        // 今回までに移動するべき距離
        float targetDistance = DASH_DISTANCE * progress;

        // 前回から今回までの移動距離
        float moveDistance = targetDistance - dashMovedDistance;

        // ダッシュ方向へ移動
        player.SetLaneOffset(
            player.GetLaneOffset()
            + dashDirection * moveDistance
        );

        // 移動済み距離を更新
        dashMovedDistance = targetDistance;

        // ダッシュ終了
        if (dashTimer >= DASH_DURATION)
        {
            isDashing = false;
        }
    }
    else
    {
        // 通常の左右移動
        if (aKey && !dKey)
        {
            player.MoveLeft(deltaTime);
        }
        else if (dKey && !aKey)
        {
            player.MoveRight(deltaTime);
        }
    }

    // コースから外れないようにする
    if (player.GetLaneOffset() < -roadLimit)
    {
        player.SetLaneOffset(-roadLimit);
    }

    if (player.GetLaneOffset() > roadLimit)
    {
        player.SetLaneOffset(roadLimit);
    }

    // 次のフレーム用に入力状態を保存
    previousAKey = aKey;
    previousDKey = dKey;
    previousSpaceKey = spaceKey;
}

// ダッシュ状態を初期化
void PlayerController::Reset()
{
    dashTimer = 0.0f;
    dashCooldownTimer = 0.0f;
    dashMovedDistance = 0.0f;

    isDashing = false;
    dashDirection = 0;
    lastMoveDirection = 0;

    previousAKey = false;
    previousDKey = false;
    previousSpaceKey = false;
}