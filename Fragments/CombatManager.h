#pragma once

#include "EnemyManager.h"
#include "BulletManager.h"
#include "PlayerProgress.h"

class CombatManager
{
private:


    //発射クールタイム
    const float shotCooldown = 5.0f;

    //現在の残りクールタイム
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
        PlayerProgress& progress
    );

    //残り時間取得
    float GetShotTimer() const;
};

