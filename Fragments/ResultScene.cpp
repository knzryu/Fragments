#include "DxLib.h"
#include "ResultScene.h"

// リザルト画面の初期化
void ResultScene::Initialize(const ResultData& data)
{
    // 結果データを保存
    resultData = data;

    // フォントを作成
    fontHandle = CreateFontToHandle(
        nullptr,    // フォント名
        48,         // 文字サイズ
        3           // 太さ
    );
}

void ResultScene::Update()
{
    // 現段階では更新処理なし
}

// リザルト画面の描画
void ResultScene::Draw()
{
    // 背景
    DrawBox(
        0, 0, 1920, 1080,
        GetColor(10, 15, 30),
        TRUE
    );

    // GAME OVER
    DrawStringToHandle(
        862, 217,
        "GAME OVER ",
        GetColor(255, 80, 80),
        fontHandle
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
    DrawStringToHandle(
        585, 420,
        "撃破数",
        GetColor(150, 200, 230),
        fontHandle
    );

    DrawFormatStringToHandle(
        675, 525,
        GetColor(255, 255, 255),
        fontHandle,
        "%d",
        resultData.defeatedEnemyCount
    );

    // 走行距離
    DrawStringToHandle(
        1125, 420,
        " 距離",
        GetColor(150, 200, 230),
        fontHandle
    );

    DrawFormatStringToHandle(
        1185, 525,
        GetColor(255, 255, 255),
        fontHandle,
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

    DrawStringToHandle(
        650, 855,
        " RETRY",
        GetColor(255, 255, 255),
        fontHandle
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

    DrawStringToHandle(
        1120, 855,
        "TITLE",
        GetColor(255, 255, 255),
        fontHandle
    );
}

// リザルト画面の終了処理
void ResultScene::Finalize()
{
    if (fontHandle != -1)
    {
        DeleteFontToHandle(fontHandle);
        fontHandle = -1;
    }
}