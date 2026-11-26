#pragma once

#include "ResultData.h"
#include <vector>

// ランキング1件分のデータ
struct RankingRecord
{
    int defeatedEnemyCount = 0;
    float distance = 0.0f;
    int score = 0;
};

class RankingManager
{
private:

    // ランキングの最大登録件数
    static constexpr int MAX_RANKING_COUNT = 5;

    // ランキングデータ
    std::vector<RankingRecord> records;

    // ファイルからランキングを読み込む
    void Load();

    // ランキングをファイルに保存する
    void Save() const;

    // スコア順に並び替える
    void SortRecords();

    // スコアを計算する
    int CalculateScore(
        int defeatedEnemyCount,
        float runDistance
    ) const;

    


public:

    // 初期化
    void Initialize();

    // リザルトデータをランキングに登録する
    void AddRecord(const ResultData& resultData);

    // ランキングデータを取得する
    const std::vector<RankingRecord>& GetRecords() const;

    // ランキングを初期化する 
    void Clear();
};