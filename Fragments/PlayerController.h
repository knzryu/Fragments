#pragma once

#include "Player.h"

class PlayerController
{
private:

	//ダッシュの設定
	static constexpr float DASH_DISTANCE = 6.0f;     // ダッシュ中の移動量（1フレームあたり）
	static constexpr float DASH_DURATION = 0.3f;  // ダッシュの持続時間（秒）
	static constexpr float DASH_COOLDOWN = 3.0f;      // ダッシュの経過時間

	//ダッシュの状態
	bool isDashing = false;
	float dashTimer = 0.0f;
	float dashCooldownTimer = 0.0f;

	// ダッシュ方向（-1：左、1：右）
	int dashDirection = 0;

	// ダッシュ開始時の移動量
	float dashMovedDistance = 0.0f;

	// 最後に押した左右キーの方向
	int lastMoveDirection = 0;

	// 前フレームのキー入力状態
	bool previousAKey = false;
	bool previousDKey = false;
	bool previousSpaceKey = false;


	//移動範囲
	float roadLimit = 8.0f;

public:

	//プレイヤーの入力と移動を更新
	void Update(Player& player, float deltaTime);

	//状態を初期化
	void Reset();
};

