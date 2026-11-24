#include "BossManager.h"
#include "CourseManager.h"

// ボスモデルの読み込み
bool BossManager::LoadModel()
{
    return boss.LoadModel();
}

// ボスモデルの解放
void BossManager::ReleaseModel()
{
    boss.ReleaseModel();
}

// ボスの状態をリセット
void BossManager::Reset()
{
    phase = BossPhase::Normal;

    nextBossDistance = 1000.0f;

    boss.Reset();
}

// ボスの更新
void BossManager::Update(
    float playerCourseZ,
    float deltaTime,
    const CourseManager& courseManager)
{
    // Normal → Warning
    if (phase == BossPhase::Normal)
    {
        if (playerCourseZ >= nextBossDistance - warningDistance)
        {
            phase = BossPhase::Warning;
        }
    }

    // Warning → Battle
    if (phase == BossPhase::Warning)
    {
        if (playerCourseZ >= nextBossDistance)
        {
            // プレイヤーの30m先、中央レーンに出現
            boss.Spawn(
                playerCourseZ + 30.0f,
                0.0f,
                courseManager
            );

            phase = BossPhase::Battle;
        }
    }


    // ボスが生存している場合のみ更新
    if (phase == BossPhase::Battle &&
        boss.IsActive() &&
        !boss.IsDead())
    {
        boss.Update(deltaTime, courseManager);
    }

    // ボス撃破判定
    if (phase == BossPhase::Battle && boss.IsDead())
    {
        // 次のボス出現距離を1000m先に設定
        nextBossDistance += 1000.0f;

        // ボスの状態をリセット
        boss.Reset();

        // 通常プレイに戻す
        phase = BossPhase::Normal;
    }
}


// 弾が命中した場合、ボスにダメージを与える
bool BossManager::ApplyBulletHit(
    float bulletPreviousCourseZ,
    float bulletCourseZ,
    float bulletLaneOffset,
    float bulletSize,
    int damage)
{
    // ボス戦中でなければ判定しない
    if (phase != BossPhase::Battle)
    {
        return false;
    }

    // ボスに命中していなければ終了
    if (!boss.IsHit(
        bulletPreviousCourseZ,
        bulletCourseZ,
        bulletLaneOffset,
        bulletSize))
    {
        return false;
    }

    // ダメージを与える
    boss.TakeDamage(damage);

    return true;
}

// レーザーがプレイヤーに命中したか判定
bool BossManager::IsPlayerHitByLaser(
    VECTOR playerMin,
    VECTOR playerMax) const
{
    // ボス戦中でなければ判定しない
    if (phase != BossPhase::Battle)
    {
        return false;
    }

    return boss.IsLaserHit(
        playerMin,
        playerMax
    );
}

// ボスの描画
void BossManager::Draw()
{
    if (phase == BossPhase::Battle &&
        boss.IsActive() &&
        !boss.IsDead())
    {
        boss.Draw();
    }
}

// ボスが戦闘中か確認する
bool BossManager::IsBossActive() const
{
    return phase == BossPhase::Battle &&
        boss.IsActive() &&
        !boss.IsDead();
}

// ボスのHPを取得する
int BossManager::GetBossHP() const
{
    return boss.GetHP();
}

// 現在の進行状況を取得
BossPhase BossManager::GetPhase() const
{
    return phase;
}

// 次のボス出現距離を取得
float BossManager::GetNextBossDistance() const
{
    return nextBossDistance;
}