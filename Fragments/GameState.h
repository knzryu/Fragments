#pragma once

enum class GameState
{
    Title,          //タイトル画面
    Playing,        // プレイ中
    AbilitySelect,  // 能力選択中
    Paused,         // ポーズ中
    GameOver,       // ゲームオーバー
    Result          // リザルト
};