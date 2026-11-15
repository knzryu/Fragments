#pragma once

enum class GameState
{
    Playing,        // プレイ中
    AbilitySelect,  // 能力選択中
    Paused,         // ポーズ中
    GameOver,       // ゲームオーバー
    Result          // リザルト
};