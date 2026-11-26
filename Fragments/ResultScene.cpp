
#include "DxLib.h"
#include "ResultScene.h"

//リザルト画面の初期化
void  ResultScene::Initialize(const ResultData& data)
{
	//結果データを保存
	resultData = data;
}

void ResultScene::Update()
{
    // 現段階では更新処理なし
}

//リザルト画面の更新
void ResultScene::Draw()
{
    // 背景
    DrawBox(
        0, 0, 1920, 1080,
        GetColor(10, 15, 30),
        TRUE
    );

    // タイトル
    DrawString(
        810, 150,
        "MISSION RESULT",
        GetColor(255, 80, 80)
    );

    DrawString(
        862, 217,
        "GAME OVER",
        GetColor(255, 80, 80)
    );

    // 結果パネル
    DrawBox(
        375, 315, 1545, 690,
        GetColor(25, 35, 55),
        TRUE
    );

    DrawBox(
        375, 315, 1545, 690,
        GetColor(80, 160, 200),
        FALSE
    );

    // 敵撃破数
    DrawString(
        585, 420,
        "DEFEATED ENEMIES",
        GetColor(150, 200, 230)
    );

    DrawFormatString(
        675, 525,
        GetColor(255, 255, 255),
        "%d",
        resultData.defeatedEnemyCount
    );

    // 走行距離
    DrawString(
        1125, 420,
        "DISTANCE",
        GetColor(150, 200, 230)
    );

    DrawFormatString(
        1185, 525,
        GetColor(255, 255, 255),
        "%.1f M",
        resultData.distance
    );

    // マウス位置
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    // RETRYボタン
    bool retryHover =
        mouseX >= 600 && mouseX <= 855 &&
        mouseY >= 825 && mouseY <= 930;

    DrawBox(
        600, 825, 855, 930,
        retryHover
        ? GetColor(40, 150, 220)
        : GetColor(20, 80, 150),
        TRUE
    );

    DrawString(
        675, 862,
        "RETRY",
        GetColor(255, 255, 255)
    );

    // TITLEボタン
    bool titleHover =
        mouseX >= 1065 && mouseX <= 1320 &&
        mouseY >= 825 && mouseY <= 930;

    DrawBox(
        1065, 825, 1320, 930,
        titleHover
        ? GetColor(40, 150, 220)
        : GetColor(20, 80, 150),
        TRUE
    );

    DrawString(
        1147, 862,
        "TITLE",
        GetColor(255, 255, 255)
    );
}