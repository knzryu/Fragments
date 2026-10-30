#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H


//プレイヤーの成長・進行状況管理クラス
class PlayerProgress
{
private:
    // 敵の撃破数
    int defeatedEnemyCount = 0;

    // 現在の経験値
    int experience = 0;

    // プレイヤーのレベル
    int playerLevel = 1;

    // 次のレベルに必要な経験値
    int nextLevelExp = 20;

public:
    // 経験値を追加する
    void AddExperience(int amount);

    // 敵の撃破数を増やす
    void AddDefeatedEnemyCount();

    // 各データを取得する
    int GetExperience() const;
    int GetPlayerLevel() const;
    int GetNextLevelExp() const;
    int GetDefeatedEnemyCount() const;

    // 初期状態に戻す
    void Reset();
};

#endif