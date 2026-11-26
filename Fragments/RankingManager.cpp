
#include "RankingManager.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

// 初期化 
void RankingManager::Initialize()
{
    Load();
}

// ファイルからランキングを読み込む 
void RankingManager::Load()
{
    records.clear();

    std::ifstream file("Ranking.txt");

    if (!file.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream stream(line);

        RankingRecord record;

        if (!(stream >> record.defeatedEnemyCount
            >> record.distance))
        {
            continue;
        }

        if (record.defeatedEnemyCount < 0 ||
            !std::isfinite(record.distance) ||
            record.distance < 0.0f)
        {
            continue;
        }

        record.score = CalculateScore(
            record.defeatedEnemyCount,
            record.distance
        );

        records.push_back(record);
    }

    SortRecords();

    if (records.size() > MAX_RANKING_COUNT)
    {
        records.resize(MAX_RANKING_COUNT);
    }
}

// ランキングをファイルに保存する 
void RankingManager::Save() const
{
    std::ofstream file("Ranking.txt");

    if (!file.is_open())
    {
        return;
    }

    for (const RankingRecord& record : records)
    {
        file << record.defeatedEnemyCount << " "
            << std::setprecision(9)
            << record.distance << "\n";
    }
}

// スコア順に並び替える 
void RankingManager::SortRecords()
{
    std::stable_sort(
        records.begin(),
        records.end(),
        [](const RankingRecord& a, const RankingRecord& b)
        {
            return a.score > b.score;
        }
    );
}

// スコアを計算する 
int RankingManager::CalculateScore(
    int defeatedEnemyCount,
    float runDistance
) const
{
    return defeatedEnemyCount * 100
        + static_cast<int>(runDistance);
}

// リザルトデータをランキングに登録する 
void RankingManager::AddRecord(const ResultData& resultData)
{
    RankingRecord record;

    record.defeatedEnemyCount = resultData.defeatedEnemyCount;
    record.distance = resultData.distance;

    record.score = CalculateScore(
        record.defeatedEnemyCount,
        record.distance
    );

    records.push_back(record);

    SortRecords();

    if (records.size() > MAX_RANKING_COUNT)
    {
        records.resize(MAX_RANKING_COUNT);
    }

    Save();
}

// ランキングデータを取得する 
const std::vector<RankingRecord>& RankingManager::GetRecords() const
{
    return records;
}

// ランキングを初期化する
void RankingManager::Clear()
{
    // メモリ上のランキングを削除
    records.clear();

    // 保存ファイルの中身を空にする
    Save();
}
