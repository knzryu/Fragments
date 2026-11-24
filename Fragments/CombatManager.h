#pragma once

#include "EnemyManager.h"
#include "BulletManager.h"
#include "PlayerProgress.h"
#include "BossManager.h"
#include "Player.h"

class CombatManager
{
private:

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
    void StartCooldown(float cooldown);


    // 敵と弾の戦闘処理
    bool Update(
        EnemyManager& enemyManager,
        BulletManager& bulletManager,
        PlayerProgress& progress,
        BossManager& bossManager,
        Player& player
    );
    

    //残り時間取得
    float GetShotTimer() const;
};

