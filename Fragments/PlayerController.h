#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Player.h"

class PlayerController
{
private:

	//ダッシュの設定
	float dashSpeed = 20.0f;     // ダッシュ中の移動量（1フレームあたり）
	float dashDuration = 0.4f;   // ダッシュの持続時間（秒）
	float dashTimer = 0.0f;      // ダッシュの経過時間

	//ダッシュの状態
	bool isDashing = false;
	bool dashUsed = false;
	int dashDirection = 0;

	//移動範囲
	float roadLimit = 8.0f;

public:

	//プレイヤーの入力と移動を更新
	void Update(Player& player, float deltaTime);

	//状態を初期化
	void Reset();
};

#endif