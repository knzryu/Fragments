#include "PlayerController.h"
#include "DxLib.h"

// プレイヤーの入力と移動を更新
void PlayerController::Update(Player& player, float deltaTime)
{
    // ダッシュ開始
    if (CheckHitKey(KEY_INPUT_SPACE) && !dashUsed)
    {
        if (CheckHitKey(KEY_INPUT_A))
        {
            dashDirection = -1;
            isDashing = true;
            dashTimer = 0.0f;
            dashUsed = true;
        }
        else if (CheckHitKey(KEY_INPUT_D))
        {
            dashDirection = 1;
            isDashing = true;
            dashTimer = 0.0f;
            dashUsed = true;
        }
    }

    // Spaceを離したら次のダッシュを許可
    if (!CheckHitKey(KEY_INPUT_SPACE))
    {
        dashUsed = false;
    }

    // ダッシュ中の処理
    if (isDashing)
    {
        // Spaceを離したら即終了
        if (!CheckHitKey(KEY_INPUT_SPACE))
        {
            isDashing = false;
        }
        else
        {
            player.SetLaneOffset(
                player.GetLaneOffset()
                + dashDirection * dashSpeed * deltaTime
            );

            dashTimer += deltaTime;

            // 持続時間を超えたら終了
            if (dashTimer >= dashDuration)
            {
                isDashing = false;
            }
        }
    }
    else
    {
        // 通常の左右移動
        if (CheckHitKey(KEY_INPUT_A))
        {
            player.MoveLeft(deltaTime);
        }

        if (CheckHitKey(KEY_INPUT_D))
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
}

// ダッシュ状態を初期化
void PlayerController::Reset()
{
    dashTimer = 0.0f;
    isDashing = false;
    dashUsed = false;
    dashDirection = 0;
}