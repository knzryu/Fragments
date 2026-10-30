#include "PlayerProgress.h"

// 経験値を追加する
void PlayerProgress::AddExperience(int amount)
{
    experience += amount;

    //必要経験値に達している間、レベルアップする
    while (experience >= nextLevelExp)
    {
        //レベルアップ
        playerLevel++;

        //次のレベルに必要な経験値を増やす
        nextLevelExp += 20;
    }
}

// 敵の撃破数を増やす
void PlayerProgress::AddDefeatedEnemyCount()
{
    defeatedEnemyCount++;
}

// 経験値を取得する
int PlayerProgress::GetExperience() const
{
    return experience;
}

// レベルを取得する
int PlayerProgress::GetPlayerLevel() const
{
    return playerLevel;
}

// 次のレベルに必要な経験値を取得する
int PlayerProgress::GetNextLevelExp() const
{
    return nextLevelExp;
}

// 敵の撃破数を取得する
int PlayerProgress::GetDefeatedEnemyCount() const
{
    return defeatedEnemyCount;
}

// 初期状態に戻す
void PlayerProgress::Reset()
{
    defeatedEnemyCount = 0;
    experience = 0;
    playerLevel = 1;
    nextLevelExp = 20;
}