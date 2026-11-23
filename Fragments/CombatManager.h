#pragma once

#include "EnemyManager.h"
#include "BulletManager.h"
#include "PlayerProgress.h"
#include "BossManager.h"
#include "Player.h"

class CombatManager
{
private:


    // 初期クールタイム
    static constexpr float INITIAL_SHOT_COOLDOWN = 5.0f;

    // クールタイムの最低値
    static constexpr float MIN_SHOT_COOLDOWN = 1.0f;

    // 強化1回あたりの短縮時間
    static constexpr float COOLDOWN_REDUCTION_AMOUNT = 0.5f;

    // 現在のクールタイム
    float shotCooldown = INITIAL_SHOT_COOLDOWN;

    // 残りクールタイム
    float shotTimer = 0.0f;


public:

    //クールタイムの初期化
    void Initialize();

    //クールタイムの更新
    void UpdateCooldown(float deltaTime);

    //発射可能か確認
    bool CanShot() const;

    //クールタイム開始
    void StartCooldown();



    // 敵と弾の戦闘処理
    bool Update(
        EnemyManager& enemyManager,
        BulletManager& bulletManager,
        PlayerProgress& progress,
        BossManager& bossManager,
        Player& player
    );
    // クールタイムを短縮
    void ReduceShotCooldown();

    //残り時間取得
    float GetShotTimer() const;
};

